#ifndef _UTIL_H
#define _UTIL_H

#include <unistd.h>
#include <fstream>
#include "Drone.h"

namespace UTIL_CONST{
	enum{

	};
}

class Config{
private:
	static std::ifstream ifs_rc;
	static std::ofstream ofs_rc;
	static std::ifstream ifs_ds;
	static std::ofstream ofs_ds;

	static int openConfigInstream();
	static int openConfigOutstream();
	static void closeConfigInstream();
	static void closeConfigOutstream();
	static int openSettingsInstream();
	static int openSettingsOutstream();
	static void closeSettingsInstream();
	static void closeSettingsOutstream();

public:
	static std::string getMail();
	static std::string getDroneNo();
	static std::string getSerialNumber();
	static PID getPidValues();
	static TRIM getTrimValues();

	static int setMail(std::string mail_value);
	static int setDroneNo(std::string dr_value);
	static int setSerialNumber(std::string sn_value);
	static int setDroneSettings(std::string set_values);
//	static int setPidValues2(std::string pid_values);
//	static int setTrimValues(std::string trim_values);
};

class Util{
private:
	static pid_t pid;
public:
	static pid_t getPid();
	static int createPidFile();
	static void	stopProcess();
	static Config conf;
//	static std::string makeHexCode(int code);
	static std::string strSplit(std::string str_origin, std::string str_tok);
	static std::string rabadonEncoder(std::string str);
	static std::string rabadonDecoder(std::string str);
};

#endif

