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

using namespace std;
using namespace CLIENT_CONST;
using namespace SPIPINO_CONST;

Client::Client(){
	spi = SpiPino::getInstance();
//	ser = Serial::getInstance();
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


int Client::client_sock;
bool Client::tcp_status = false;

int Client::connectServer(){
	int is_login = 0;
	struct sockaddr_in server_addr;

	if((client_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		cout << "fail to create socket" << endl;
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(kPort);
	server_addr.sin_addr.s_addr = inet_addr(kServerAddr);
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		cout << "failed to connect to the server" << endl;
		close(client_sock);
		return -1;
	}

	char code[5];
	// 서버에 드론 연결 됐다고 보내기
	sprintf(code, "%d}", SEND_ACCOUNT);
	if(send(client_sock, code, sizeof(code), 0) <= 0){
		cout << "failed to send server." << endl;
		return -1;
	}
	char rBuf[4] = {0};
	if(recv(client_sock, rBuf, sizeof(rBuf), 0) <= 0){
		cout << "failed to connect to the server." << endl;
		close(client_sock);
		return -1;
	} else if(rBuf[0] == '1' && rBuf[1] == '0' && rBuf[2] == '0'){
		cout << "connected to the server." << endl;
	}

	string user_mail = Util::conf.getMail();
	string drone_no = Util::conf.getDroneNo();
	// mail 또는 drone no가 없다면
	if(user_mail == "" || drone_no == ""){
		// serial number가 없으면 서버에 요청해서 받아오기
		string sNum = Util::conf.getSerialNumber();
		if(sNum == ""){
			// serial number가 없다면 요청
			memset(code ,0, sizeof(code));
			sprintf(code, "%d}", REQ_SERIAL);
			if(send(client_sock, code, sizeof(code), 0) < 0){
				cout << "failed to request serial number." << endl;
				close(client_sock);
				return -1;
			}
			char recv_sNum[30] = {0};
	        if(recv(client_sock, recv_sNum, 30, 0) < 0){
	            cout << "failed to receive serial number." << endl;
	            close(client_sock);
	            return -1;
	        }
			if(strstr(recv_sNum, "(no_serial)")){
				close(client_sock);
				sleep(30);
				return -1;
			}
			Util::conf.setSerialNumber(recv_sNum);
		}

		// mail이 없다면 요청
		if(user_mail == ""){
			string req_mail = REQ_MAIL + "," + Util::conf.getSerialNumber() + "}";
		//	strncat(req_mail, Util::conf.getSerialNumber().c_str(), 30);
			if(send(client_sock, req_mail.c_str(), req_mail.length(), 0) < 0){
				cout << "failed to request mail." << endl;
				close(client_sock);
				return -1;
			}
			char recv_mail[60] = {0};
			if(recv(client_sock, recv_mail, 60, 0) < 0){
				cout << "failed to receive mail." << endl;
				close(client_sock);
				return -1;
			}
			if(strstr(recv_mail, "(no_mail)")){
				close(client_sock);
				sleep(30);
				return -1;
			}
			Util::conf.setMail(recv_mail);
		}

		// 드론 번호가 없다면 요청
		if(drone_no == ""){
			string req_dNo = REQ_DRONE_NO + "," + Util::conf.getSerialNumber() + "}";
	//		strncat(req_dNo, Util::conf.getSerialNumber().c_str(), 30);
			if(send(client_sock, req_dNo.c_str(), req_dNo.length(), 0) < 0){
				cout << "failed to request drone no." << endl;
				close(client_sock);
				return -1;
			}
			char recv_dNo[30] = {0};
			if(recv(client_sock, recv_dNo, 30, 0) < 0){
				cout << "failed to receive drone no." << endl;
				close(client_sock);
				return -1;
			}
			if(strstr(recv_dNo, "(no_drone)")){
				cout << "not registered yet." << endl;
				close(client_sock);
				sleep(30);
				return -1;
			}
			Util::conf.setDroneNo(recv_dNo);
		}
	}

	// 서버에 메일 보내기
	sprintf(code, "%d", SEND_MAIL);
	string submit_mail = string(code) + "," + Util::conf.getMail() + "}";
	if(send(client_sock, submit_mail.c_str(), submit_mail.length(), 0) < 0){
		cout << "failed to send mail to server." << endl;
		close(client_sock);
		return -1;
	}
	memset(rBuf, 0, sizeof(rBuf));
	if(recv(client_sock, rBuf, 4, 0) <= 0){
		cout << "failed to receive mail response." << endl;
		close(client_sock);
		return -1;
	} else if(rBuf[0] == '1' && rBuf[1] == '1' && rBuf[2] == '3'){
		cout << "mail response received." << endl;
	}
	//서버에 드론 번호 보내기
	sprintf(code, "%d", SEND_DRONE_NO);
	string submit_dNo = string(code) + "," + Util::conf.getDroneNo() + "}";
	if(send(client_sock, submit_dNo.c_str(), submit_dNo.length(), 0) < 0){
		cout << "failed to send drone_no to server." << endl;
		close(client_sock);
		return -1;
	}
	memset(rBuf, 0, sizeof(rBuf));
	if(recv(client_sock, rBuf, 4, 0) <= 0){
		cout << "failed to receive drone number response." << endl;
		close(client_sock);
		return -1;
	} else if(rBuf[0] == '1' && rBuf[1] == '1' && rBuf[2] == '4'){
		cout << "drone number response received." << endl;
	}
	// PID 받아오기
	sprintf(code, "%d}", RECV_SETTING);
	send(client_sock, code, sizeof(code), 0);
	char settings_buf[4096] = {0};
	if(recv(client_sock, settings_buf, sizeof(settings_buf), 0) > 0){
		cout << "settings are received." << endl;
		Util::conf.setDroneSettings(settings_buf);
		sendSettingsToIno(Util::rabadonDecoder(settings_buf));
//#TODO		Ahrs::applyPidValues(Util::conf.getPidValues());
//#TODO		Ahrs::applyTrimValues(Util::conf.getTrimValues());
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
			sprintf(code, "%d}", ALIVE_CODE);
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
				if(recv_buf[i] == '}'){
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
		sendSettingsToIno(Util::rabadonDecoder(data));
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



