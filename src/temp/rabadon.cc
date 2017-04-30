#include <iostream>
#include <thread>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include "Util.h"
#include "Drone.h"
#include "Client.h"
#include "Ahrs.h"

using namespace std;

void sockConnectNazeRev6(Client sock);
void connectionChecker(NazeRev6 nz6);
void mainThread();

void setGain();

int main(int argc, char* argv[]){

//	NazeRev6 nz6;
//	Client sock(nz6);

	signal(SIGPIPE, SIG_IGN);

	if(argc == 1 || (argc == 2 && strstr(argv[1], "start"))){
//		thread socket_thread(sockConnectNazeRev6, sock);
//		socket_thread.detach();

		Util::createPidFile();
		mainThread();
	//	connectionChecker(nz6);
	} else if(argc == 2 && strstr(argv[1], "stop")){
		Util::stopProcess();
	} else if(argc == 4 && strstr(argv[1], "-a")){
		string email = argv[2];
		string drone_no = argv[3];
		Util::conf.setMail(email);
		Util::conf.setDroneNo(drone_no);
	//	sock.connectServer();
	} else{
//		Ahrs::setKalmanValues(strtod(argv[1], NULL), strtod(argv[2], NULL), strtod(argv[3], NULL));


	//	thread socket_thread(sockConnectNazeRev6, sock);
	//	socket_thread.detach();

	//	Util::createPidFile();
	//	mainThread();

//		cout << "Usage: rabadon [-a email name]" << endl;
//		cout << "Options:" << endl;
//		cout << "  [-a email name]	: register drone information for server" << endl;
	}

	return 0;
}
/*
void sockConnectNazeRev6(Client sock){
	while(1){
		if(sock.connectServer() != -1){
			// this thread must be executed before recvCode()
		//	thread codeHandler_th(NazeRev6::codeHandler);
		//	codeHandler_th.detach();

			sock.recvCode();
		}
		sleep(1);
	}
}
*/
void mainThread(){
	Ahrs ar;
	while(true){
	//	Ahrs::pidProc();
		ar.ahrsProc();
	}
}














void connectionChecker(NazeRev6 nz6){
	int naze_disconn = 0;
	while(true){
		if(nz6.conn_flag == false){
			if(nz6.com != -1){
				cout << nz6.kDevName << " was detached." << endl;
				nz6.code_status[nz6.sFc] = false;
				nz6.detached();
				close(nz6.com);
			}
			nz6.listen();
			nz6.connect();
			nz6.conn_flag = true;

	///		thread pidProc_thread(Ahrs::pidProc);
	///		pidProc_thread.detach();

		//	thread codeReader_th(NazeRev6::codeReader);
		//	codeReader_th.detach();

			cout << nz6.kDevName << " was attached." << endl;
			nz6.code_status[nz6.sFc] = true;
		}

		if(write(nz6.com, " ", 1) < 0){	
			if(naze_disconn++ > 2) nz6.conn_flag = false;
		} else{
			naze_disconn = 0;
			nz6.conn_flag = true;
		}

		sleep(1);
	}
}


