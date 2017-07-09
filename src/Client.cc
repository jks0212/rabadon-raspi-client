#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "Client.h"
#include "Util.h"

//#include "Serial.h"
#include "SpiPino.h"
#include "json/json.h"
#include "data/Data.h"
#include "setting/Setting.h"
#include "http/SimpleHttp.h"

using namespace std;
using namespace CLIENT_CONST;
using namespace SPIPINO_CONST;

Client::Client(){
//	spi = SpiPino::getInstance();
	spi = NULL;
}


bool Client::code_status[100];
/*
char Client::cRegisterAccount[kCodeBufSize] = "100";      // send code
char Client::cUserAccess[kCodeBufSize] = "101";          // receive code
char Client::cUserExit[kCodeBufSize] = "102";            // receive code
char Client::cControlSet[kCodeBufSize] = "350";		// receive code
char Client::cPidSet[kCodeBufSize] = "351";			// receive code
char Client::cErrAccount[kCodeBufSize] = "401";          // send code
char Client::cErrFcComm[kCodeBufSize] = "402";           // send code
char Client::cAlive[kCodeBufSize] = "500";           // receive send code
char Client::cFcAttached[kCodeBufSize] = "501}";		// send code
char Client::cFcDetached[kCodeBufSize] = "502}";		// send code
*/
const char Client::kServerAddr[] = "45.32.249.203";
const string Client::P2P_SERVER_ADDR = "13.124.102.238";
const string Client::API_SERVER_ADDR = "13.124.102.238";

string Client::drone_id;
DronePid Client::dronePid;
DroneTrim Client::droneTrim;

int Client::client_sock;
bool Client::tcp_status = false;


int Client::checkSettings(){
	string user_mail = Util::conf.getMail();
	string drone_id = Util::conf.getDroneId();
}

int Client::connectServer(){

	SimpleHttp simpleHttp;
	Setting setting;

	string serial_num = Setting::getSerialNumber();
	if(serial_num.empty()){
		simpleHttp.addHeader("Content-Type", "application/json");
		HttpResponse* res = simpleHttp.post(API_SERVER_ADDR, "/v1/drone/serial");
		if(res == NULL){
			cout << "Client->connectServer(): Fail to request serial number." << endl;
			return -1;
		} else{
			if(res->code != 204){
				cout << "Client->connectServer(): " << res->json["error"] << endl;
			}

			if(res->json["serial"] == Json::nullValue){
				cout << "Client->connectServer(): Fail to get a new serial." << endl;
				return -1;
			}

			try{
				serial_num = res->json["serial"].asString();
			} catch(Json::LogicError const & e){
				cout << "Client->connectServer(): " << e.what() << endl;
				return -1;
			}
			if(setting.setSerialNumber(serial_num) < 0){
				cout << "Client->connectServer(): Fail to set serial." << endl;
				return -1;
			}
		}
	}


	HttpResponse* res;
//	string drone_id;
	res = simpleHttp.get(API_SERVER_ADDR, "/v1/drone/serial/" + serial_num);
	if(res == NULL){
		cout << "Client->connectServer(): Fail to request drone id" << endl;
		return -1;
	} else{
		if(res->code != 200){
			cout << "Client->connectServer(): " << res->json["error"] << endl;
			return -1;
		}

		if(res->json["drone_id"] == Json::nullValue){
			cout << "Client->connectServer(): Fail to get a drone id" << endl;
			return -1;
		}

		try{
			drone_id = res->json["drone_id"].asString();
		} catch(Json::LogicError const & e){
			cout << "Client->connectServer(): " << e.what() << endl;
			return -1;
		}
	}
	DronePid dronePid;
	DroneTrim droneTrim;
	res = simpleHttp.get(API_SERVER_ADDR, "/v1/drone/" + drone_id);
	if(res == NULL){
		cout << "Client->connectServer(): Fail to request drone PID and Trim" << endl;
		return -1;
	} else{
		if(res->code != 200){
			cout << "Client->connectServer(): " << res->json["error"] << endl;
			return -1;
		}

		if(res->json["drone"] == Json::nullValue){
			cout << "Client->connectServer(): Fail to get a drone PID and Trim" << endl;
			return -1;
		}

		try{
			dronePid.roll_outer_p = res->json["drone"]["pid"]["roll"]["outer_p"].asDouble();
			dronePid.roll_p = res->json["drone"]["pid"]["roll"]["p"].asDouble();
			dronePid.roll_i = res->json["drone"]["pid"]["roll"]["i"].asDouble();
			dronePid.roll_d = res->json["drone"]["pid"]["roll"]["d"].asDouble();
			dronePid.pitch_outer_p = res->json["drone"]["pid"]["pitch"]["outer_p"].asDouble();
			dronePid.pitch_p = res->json["drone"]["pid"]["pitch"]["p"].asDouble();
			dronePid.pitch_i = res->json["drone"]["pid"]["pitch"]["i"].asDouble();
			dronePid.pitch_d = res->json["drone"]["pid"]["pitch"]["d"].asDouble();
			dronePid.yaw_outer_p = res->json["drone"]["pid"]["yaw"]["outer_p"].asDouble();
			dronePid.yaw_p = res->json["drone"]["pid"]["yaw"]["p"].asDouble();
			dronePid.yaw_i = res->json["drone"]["pid"]["yaw"]["i"].asDouble();
			dronePid.yaw_d = res->json["drone"]["pid"]["yaw"]["d"].asDouble();
			droneTrim.roll = res->json["drone"]["trim"]["roll"].asDouble();
			droneTrim.pitch = res->json["drone"]["trim"]["pitch"].asDouble();
			droneTrim.yaw = res->json["drone"]["trim"]["yaw"].asDouble();
		} catch(Json::LogicError const & e){
			cout << "Client->connectServer(): " << e.what() << endl;
			return -1;
		}
	}



	struct sockaddr_in server_addr;

	if((client_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		cout << "Client->connectServer(): Fail to create P2P socket." << endl;
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(P2P_SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(P2P_SERVER_ADDR.c_str());
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		cout << "Client->connectServer(): Fail to connect to P2P server." << endl;
		return -1;
	}
/*
	string identity = "100";
	if(send(client_sock, identity.c_str(), identity.length(), 0) < 0){
		cout << "Client->connectServer(): Fail to send identity to server." << endl;
		close(client_sock);
		return -1;
	}
*/
	char code[4];
	sprintf(code, "%d", SEND_DRONE_ID);
	string code_drone_id = string(code) + drone_id + '\n';
	if(send(client_sock, code_drone_id.c_str(), code_drone_id.length(), 0) < 0){
		cout << "Client->connectServer(): Fail to send DroneID to server." << endl;
		close(client_sock);
		return -1;
	}

	char buff[100];
	if(recv(client_sock, buff, 100, 0) < 0){
		cout << "Client->connectServer(): Fail to receive CODE DroneID response" << endl;
		close(client_sock);
		return -1;
	}

	string _buff = buff;
	string code_res = _buff.substr(0, _buff.find('\n'));
	if(code_res.compare("200") != 0){
		cout << "Client->connectServer(): P2P Server -> (" + code_res + ")" << endl;
		close(client_sock);
		return -1;
	}

	// for Non-Block socket
	fcntl(client_sock, F_SETFL, O_NONBLOCK);

	tcp_status = true;

	return 0;
}


void Client::recvCode(){
	char recv_buf[kRecvBufSize];

	double alive_cycle = 0.7;
	double alive_limit = 3;
	double _alive_cycle = 0;
	double _alive_limit = 0;

	double loop_cycle;
	clock_t eTime = clock();

	while(true){
		loop_cycle = ((double)(clock() - eTime)) / CLOCKS_PER_SEC;
		eTime = clock();

		if(code_status[STAT_ALIVE] == true){
			// connection is alive
			_alive_limit = 0;
			code_status[STAT_ALIVE] = false;
		}

		_alive_cycle += loop_cycle;
		if(alive_cycle <= _alive_cycle){
			char code[5];
			sprintf(code, "%d\n", ALIVE_CODE);
			send(client_sock, code, strlen(code), 0);
			_alive_cycle = 0;
		}

		_alive_limit += loop_cycle;
		if(alive_limit <= _alive_limit){
			tcp_status = false;
			cout << "disconnected from the server." << endl;
			close(client_sock);
			break;
		}

		memset(recv_buf, 0, kRecvBufSize);
		// 소켓 데이터 받기 실패하면 continue
		int rLen = 0;
		if((rLen = recv(client_sock, recv_buf, kRecvBufSize, 0)) <= 0) continue;

		char rBuf[kRecvBufSize] = {0};
		for(int i=0; i<rLen; i++){
			memset(rBuf, 0, sizeof(rBuf));
			for(int k=0; k<rLen; k++, i++){
				if(recv_buf[i] == '\n'){
					i++;
					break;
				}
				rBuf[k] = recv_buf[i];
			}

			int code;
			sscanf(rBuf, "%d", &code);
			if(code == ALIVE_CODE){
				 _alive_limit = 0;
			} else{
				recvCodeHandler(rBuf);
			}
		}

	}
}


void Client::recvCodeHandler(char code_data[]){
	string cd = code_data;
	if(cd.length() < 2) return;
	int code;
	sscanf(cd.substr(0, 3).c_str(), "%d", &code);
	string data = cd.substr(3);

	if(code == RECV_CONTROL){
		if(cd.length() < 11) return;
		string ctrl = cd.substr(0, 15);
		cout << "ctrl : " << ctrl << endl;
		sendControlToIno(data);
	} else if(code == RECV_SETTING){
	//	sendSettingsToIno(Util::rabadonDecoder(data));
		sendSettingsToIno(dronePid);
		cout << cd << endl;
	}
}

void Client::sendControlToIno(string data){
	uint8_t roll = 0;
	int r_ang = 10 * (data[1] - 48) + (data[2] - 48);
	if(data[0] == '1') roll |= 128;
	roll |= r_ang;
	spi->transfer(SET_ROLL, roll);

	uint8_t pitch = 0;
	int p_ang = 10 * (data[4] - 48) + (data[5] - 48);
	if(data[3] == '1') pitch |= 128;
	pitch |= p_ang;
	spi->transfer(SET_PITCH, pitch);

	uint8_t yaw = 0;
	int y_ang = 10 * (data[7] - 48) + (data[8] - 48);
	if(data[6] == '1') yaw |= 128;
	yaw |= y_ang;
	spi->transfer(SET_YAW, yaw);

	uint8_t power = 0;
	int p = 100 * (data[9] - 48) + 10 * (data[10] - 48) + (data[11] - 48);
	power |= p;
	spi->transfer(SET_POWER, power);
}

void Client::sendSettingsToIno(DronePid pid){
	spi->transfer(SET_ROLL_OUTER_P, pid.roll_outer_p);
	spi->transfer(SET_ROLL_P, pid.roll_p);
	spi->transfer(SET_ROLL_I, pid.roll_i);
	spi->transfer(SET_ROLL_D, pid.roll_d);
	spi->transfer(SET_PITCH_OUTER_P, pid.pitch_outer_p);
	spi->transfer(SET_PITCH_P, pid.pitch_p);
	spi->transfer(SET_PITCH_I, pid.pitch_i);
	spi->transfer(SET_PITCH_D, pid.pitch_d);
	spi->transfer(SET_YAW_OUTER_P, pid.yaw_outer_p);
	spi->transfer(SET_YAW_P, pid.yaw_p);
	spi->transfer(SET_YAW_I, pid.yaw_i);
	spi->transfer(SET_YAW_D, pid.yaw_d);

}

void Client::sendSettingsToIno(string data){
	Json::Value root;
	Json::Reader reader;
	if(!reader.parse(data, root)){
		cout << "Client::sendSettingsToIno(): failed to parse settings." << endl;
		return;
	}

	Json::Value pid = root["pid"];
	Json::Value roll = pid["roll"];
	Json::Value pitch = pid["pitch"];
	Json::Value yaw = pid["yaw"];
	double roll_outer_p = (double)strtod(roll["outer_p"].asString().c_str(), NULL);
	double roll_p = (double)strtod(roll["p"].asString().c_str(), NULL);
	double roll_i = (double)strtod(roll["i"].asString().c_str(), NULL);
	double roll_d = (double)strtod(roll["d"].asString().c_str(), NULL);
	double pitch_outer_p = (double)strtod(pitch["outer_p"].asString().c_str(), NULL);
	double pitch_p = (double)strtod(pitch["p"].asString().c_str(), NULL);
	double pitch_i = (double)strtod(pitch["i"].asString().c_str(), NULL);
	double pitch_d = (double)strtod(pitch["d"].asString().c_str(), NULL);
	double yaw_outer_p = (double)strtod(yaw["outer_p"].asString().c_str(), NULL);
	double yaw_p = (double)strtod(yaw["p"].asString().c_str(), NULL);
	double yaw_i = (double)strtod(yaw["i"].asString().c_str(), NULL);
	double yaw_d = (double)strtod(yaw["d"].asString().c_str(), NULL);

	spi->transfer(SET_ROLL_OUTER_P, roll_outer_p);
	spi->transfer(SET_ROLL_P, roll_p);
	spi->transfer(SET_ROLL_I, roll_i);
	spi->transfer(SET_ROLL_D, roll_d);
	spi->transfer(SET_PITCH_OUTER_P, pitch_outer_p);
	spi->transfer(SET_PITCH_P, pitch_p);
	spi->transfer(SET_PITCH_I, pitch_i);
	spi->transfer(SET_PITCH_D, pitch_d);
	spi->transfer(SET_YAW_OUTER_P, yaw_outer_p);
	spi->transfer(SET_YAW_P, yaw_p);
	spi->transfer(SET_YAW_I, yaw_i);
	spi->transfer(SET_YAW_D, yaw_d);
}



