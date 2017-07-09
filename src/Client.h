#include "Drone.h"
#include "SpiPino.h"
#include "data/Data.h"

#ifndef _CLIENT_H
#define _CLIENT_H

namespace CLIENT_CONST{
	enum{
		STAT_USER, STAT_ALIVE, STAT_CONTROL,
	//	SEND_ACCOUNT = 100,
		SEND_DRONE_ID = 100,
		USER_ACCESS = 101,
		REQ_SERIAL = 110,
		REQ_MAIL = 111,
		REQ_DRONE_NO = 112,
		SEND_MAIL = 113,
		SEND_DRONE_NO = 114,
		USER_EXIT = 102,
		RECV_CONTROL = 350,
		RECV_SETTING = 351,
		ALIVE_CODE = 500
	};
}


class Client{
public:
	Client();
//	Client(Drone _dr);
private:
	SpiPino* spi;

	static const std::string API_SERVER_ADDR;
	static const char kServerAddr[];//[20] = "45.32.249.203";
	static const int kPort = 81;
	static const std::string P2P_SERVER_ADDR;
	static const int P2P_SERVER_PORT = 81;

	static std::string drone_id;
	static DronePid dronePid;
	static DroneTrim droneTrim;

	const unsigned short kReadBufSize = 4096;

	static const unsigned short kRecvBufSize = 4096;

	static std::string user_id;
//	static std::string drone_id;

	int checkSettings();
	void recvCodeHandler(char code_data[]);
	void sendToIno(char code_data[]); // will be deprecaed
	void sendControlToIno(std::string data);
	void sendSettingsToIno(std::string data);


	void sendSettingsToIno(DronePid pid);
/*
	static const unsigned short kMotorBufSize = 44;
	static const unsigned short kCodeLen = 3;

	static char motor_buf[];
	static bool code_status[];
	static const unsigned short sUser;		// true: user access, false: user leave
	static const unsigned short sMotorSet;		// true: set motor, false: none
	static const unsigned short sAccelGyroReq;	// true: start, false: stop
	static const unsigned short sBarometerReq;	// true: start, false: stop
	static const unsigned short sGpsReq;		// true: start, false: stop
	static const unsigned short sAlive;		// true: received, false: not received

	// send datas from drone to server cycle
	static unsigned int loop_cycle;
	static unsigned int accel_gyro_cycle;
	static unsigned int barometer_cycle;
	static unsigned int gps_cycle;
	static const unsigned int alive_cycle;
	static const unsigned int alive_limit;

	void codeBufSet(char *code_buf);
*/
public:
	static char cRegisterAccount[];      // send code
	static char cUserAccess[];          // receive code
	static char cUserExit[];            // receive code
//	static const char cMotorSet[];             // receive code
//	static const char cStartAccelGyro[];      // receive code
//	static const char cStopAccelGyro[];       // receive code
//	static const char cStartBarometer[];       // receive code
//	static const char cStopBarometer[];       // receive code
//	static const char cStartGps[];            // receive code
//	static const char cStopGps[];             // receive code
	static char cControlSet[];		//receive code
	static char cPidSet[];			// receive code
//	static const char cSendAccelGyro[];       // send code
//	static const char cSendBarometer[];       // send code
//	static const char cSendGps[];             // send code
	static char cErrAccount[];          // send code
	static char cErrFcComm[];           // send code
	static char cAlive[];           // receive send code
	static char cFcAttached[];	// send code
	static char cFcDetached[];	// send code


	static const unsigned int kCodeLen = 3;
	static const unsigned int kCodeBufSize = 5;
	static bool code_status[];
/*
	static const int sUser = 0;
	static const int sMotorSet = 1;
	static const int sAccelGyroReq = 2;
	static const int sBarometerReq = 3;
	static const int sGpsReq = 4;
	static const int sAlive = 5;
	static const int sFc = 6;
	static const int sControlSet = 7;
*/
//	void setCodeStatus(char *code_buf);

	static int client_sock;
	static bool tcp_status;

	int connectServer();
	void recvCode();
};

#endif

