#include <iostream>
#include <thread>
#include <cstring>
//#include <cstdlib>
//#include <unistd.h>
#include <signal.h>
//#include <sys/types.h>
//#include <sys/time.h>
//#include <sys/socket.h>
#include "Util.h"
#include "Client.h"
//#include "Serial.h"
//#include "I2cPi.h"
#include "SpiPino.h"

#include "json/json.h"

using namespace std;
using namespace SPIPINO_CONST;

void tcpThread();
void serialThread();
void enableModem();

int main(int argc, char* argv[]){
	signal(SIGPIPE, SIG_IGN);

	tcpThread();

	if(argc == 1 || (argc == 2 && strstr(argv[1], "start"))){
		Util::createPidFile();

		thread serial_thread(serialThread);
		serial_thread.detach();
	//	tcpThread();
	} else if(argc == 2 && strstr(argv[1], "stop")){
		Util::stopProcess();
	} else if(argc == 4 && strstr(argv[1], "-a")){
		string email = argv[2];
		string drone_no = argv[3];
		Util::conf.setMail(email);
		Util::conf.setDroneNo(drone_no);
	}

	return 0;
}

void tcpThread(){
	Client clnt;
	while(true){
		enableModem();
		sleep(2);
		if(clnt.connectServer() != -1){
			clnt.recvCode();
		}
		sleep(7);
	}
}

void serialThread(){
	SpiPino* spi = SpiPino::getInstance();
//	Serial* ser = Serial::getInstance();
//	I2cPi* ipi = I2cPi::getInstance();
	char buf[100];
//	string buf;
	while(true){
		return;
	//	spi->transfer(SET_ROLL, 30);
	//	sleep(1);
	//	spi->transfer(SET_P, 123.456);
	//	sleep(1);
	//	spi->transfer(SET_D, 0.789);
	//	sleep(1);
		/*
		ipi->i2cWrite(SET_ROLL, 30);
		sleep(1);
		ipi->i2cWrite(SET_ROLL, 123.456);
		sleep(1);
		ipi->i2cWrite(SET_ROLL, 0.1234);
		sleep(1);
		*/
		/*
		unsigned char* r = ipi->i2cRead();
		if(r != NULL){
			for(int i=0; i<10; i++){
				printf("%d ", r[i]);
			}
			printf("\n");
		}
		*/
	}
}

void enableModem(){
	FILE* pipe = popen("usb_modeswitch -J -v 0x12d1 -p0x1f01", "r");
	if(!pipe){
		cout << "Fail to enable modem." << endl;
	}
}










