#ifndef _SETTING_H
#define _SETTING_H

#include <fstream>

namespace SETTING_CONST{
	enum{

	};
}

class Setting{
	private:
		static std::string EMPTY_STR;

		static std::ifstream cIfsSetting;
		static std::ofstream cOfsSetting;

	public:
		static int openSettingInstream();
		static int openSettingOutstream();
		static void closeSettingInstream();
		static void closeSettingOutstream();

//	public:
		static std::string getUserId();
		static std::string getDroneId();
		static std::string getSerialNumber();
//		static Pid gtPidValues();
//		static Trim getTrimValues();

		static int setUserId(std::string user_id);
		static int setDroneNo(std::string dr_value);
		static int setSerialNumber(std::string serial_num);
		static int setDroneSettings(std::string set_values);

};

/*
class Config{
private:
	static std::ifstream ifs_rc;
	static std::ofstream ofs_rc;
	//deprecated
	static std::ifstream ifs_ds;
	static std::ofstream ofs_ds;

	static std::ifstream ifs_settings;
	static std::ofstream ofs_settings;

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
	static std::string getDroneId();
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
*/
#endif

