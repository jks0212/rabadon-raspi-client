#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Util.h"
#include "json/json.h"

using namespace std;

// Util Class Methods
pid_t Util::pid = getpid();
Config Util::conf;

pid_t Util::getPid(){
	return pid;
}

int Util::createPidFile(){
	ofstream ofs;
	ofs.open("/run/rabadon.pid");
	if(ofs.is_open() == false){
		cout << "failed to write pid file." << endl;
	} else{
		ofs << pid;
		ofs.close();
	}
}

void Util::stopProcess(){
	FILE *pid_file;
	FILE *pp;
	FILE *kp;
	char pid_buf[20];
	char kill[30] = "kill ";

	// pidfile열기
	if(!(pid_file = fopen("/run/rabadon.pid", "r"))){
		perror("pidfile");
		exit(1);
	}

	// pidfile에서 pid 얻고 프로세스 죽이기
	if(!fgets(pid_buf, 20, pid_file)){
		perror("pidfile");
		exit(1);
	}
	strncat(kill, pid_buf, 20);
	kp = popen(kill, "r");

	// pidfile삭제
	if(!(pp = popen("rm -f /run/rabadon/rabadon.pid", "r"))){
		perror("pidfile");
		exit(1);
	}

	pclose(kp);
	pclose(pp);
	fclose(pid_file);
}

string Util::rabadonEncoder(string str){
	string result = "";
	for(int i=0; i<str.length(); i++){
		char ch = str.at(i);
		if(ch == '}'){
			result += "%7D";
		} else if(ch == '{'){
			result += "%7B";
		} else{
			result += ch;
		}
	}

	return result;
}

string Util::rabadonDecoder(string str){
	string result = "";
	int len;
	for(int i=0; i<str.length() - 2; i++){
		len = 0;
		char ch = str.at(i);
		if(ch == '%'){
			char ch1 = str.at(i+1);
			char ch2 = str.at(i+2);
			if(ch1 == '7'){
				if(ch2 == 'B'){
					result += "{";
					i = i+2;
					len = i;
				} else if(ch2 == 'D'){
					result += "}";
					i = i+2;
					len = i;
				}
			}
		}
		if(len == 0){
			result += ch;
			len = i;
		}
	}

	int remain = str.length() - 1 - len;
	if(remain == 1) result += str.at(str.length() - 1);
	else if(remain == 2){
		result += str.at(str.length() - 2);
		result += str.at(str.length() - 1);
	}

	return result;
}




// Config Class

ifstream Config::ifs_rc;
ofstream Config::ofs_rc;

//Deprecated
ifstream Config::ifs_ds;
ofstream Config::ofs_ds;

ifstream Config::ifs_settings;
ofstream Config::ofs_settings;


int Config::openConfigInstream(){
	ifs_rc.open("/usr/local/etc/rabadon/settings.json", ios::in);
	if(ifs_rc.is_open() == false){
		cout << "failed to open settings.json file." << endl;
		return -1;
	}
}

int Config::openConfigOutstream(){
	ofs_rc.open("/usr/local/etc/rabadon/config.json", ios::trunc);
	if(ofs_rc.is_open() == false){
		cout << "failed to write rabadon-conf.json file." << endl;
		return -1;
	}

}

void Config::closeConfigInstream(){
	ifs_rc.close();
}

void Config::closeConfigOutstream(){
	ofs_rc.close();
}

int Config::openSettingsInstream(){
	ifs_settings.open("settings.json", ios::in);
	if(ifs_settings.is_open() == false){
		cout << "Fail to open settings.json." << endl;
		return -1;
	}
}

int Config::openSettingsOutstream(){
	ofs_settings.open("settings.json", ios::trunc);
	if(ofs_settings.is_open() == false){
		cout << "Fail to write settings.json." << endl;
		return -1;
	}
}

void Config::closeSettingsInstream(){
	ifs_settings.close();
}

void Config::closeSettingsOutstream(){
	ofs_settings.close();
}


string Config::getMail(){
	if(openSettingsInstream() == -1) return "";
	string rs = "";
	string temp;
	while(getline(ifs_settings, temp)){
		rs += temp;
	}
	closeSettingsInstream();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "getMail(): Fail to parse json." << endl;
		return "";
	}

	Json::Value jsonVal = root["user"]["mail"];
	if(jsonVal == Json::nullValue){
		cout << "getMail(): Fail to parse json." << endl;
		return "";
	}
	Json::FastWriter fastWriter;
	string mail = fastWriter.write(jsonVal);

	return mail;
}

string Config::getDroneId(){
	if(openSettingsInstream() == -1) return "";
	string rs;
	string temp;
	while(getline(ifs_settings, temp)){
		rs += temp;
	}
	closeSettingsInstream();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "getDroneId(): Fail to parse json." << endl;
		return "";
	}

	Json::Value jsonVal = root["drone"]["id"];
	if(jsonVal == Json::nullValue){
		cout << "getDroneId(): Fail to parse json." << endl;
		return "";
	}
	Json::FastWriter fastWriter;
	string drone_id = fastWriter.write(jsonVal);

	return drone_id;
}

string Config::getSerialNumber(){
	if(openSettingsInstream() == -1) return "";
	string rs;
	string temp;
	while(getline(ifs_settings, temp)){
		rs += temp;
	}
	closeSettingsInstream();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "getSerialNumber(): Fail to parse json." << endl;
		return "";
	}

	Json::Value jsonVal = root["serial"]["number"];
	if(jsonVal == Json::nullValue){
		cout << "getSerialNumber(): Fail to parse json." << endl;
		return "";
	}
	Json::FastWriter fastWriter;
	string serial_num = fastWriter.write(jsonVal);

	return serial_num;


	/*
	if(openConfigInstream() == -1) return "";
	string sn;
	string temp;
	while(!ifs_rc.eof()){
		getline(ifs_rc, temp);
		if(temp.find("serial_number=") != string::npos){
			sn = temp.substr(14);
			break;
		}
	}
	closeConfigInstream();
	return sn;
	*/
}

PID Config::getPidValues(){
	PID drone_pid;
//	Ahrs::initPidValues(&drone_pid);
	if(openSettingsInstream() == -1) return drone_pid;
	string settings = "";
	while(!ifs_ds.eof()){
		string temp;
		getline(ifs_ds, temp);
		settings += temp;
	}
	
	Json::Value root;
	Json::Reader reader;
	if(!reader.parse(settings, root)){
		cout << "getPidValues(): failed to parse settings." << endl;
	}

	Json::Value pid = root["pid"];
	Json::Value trim = root["trim"];
	Json::Value roll = pid["roll"];
	Json::Value pitch = pid["pitch"];
	Json::Value yaw = pid["yaw"];

	drone_pid.roll_outer_p = (double)strtod(roll["outer_p"].asString().c_str(), NULL);
	drone_pid.roll_p = (double)strtod(roll["p"].asString().c_str(), NULL);
	drone_pid.roll_i = (double)strtod(roll["i"].asString().c_str(), NULL);
	drone_pid.roll_d = (double)strtod(roll["d"].asString().c_str(), NULL);
	drone_pid.pitch_outer_p = (double)strtod(pitch["outer_p"].asString().c_str(), NULL);
	drone_pid.pitch_p = (double)strtod(pitch["p"].asString().c_str(), NULL);
	drone_pid.pitch_i = (double)strtod(pitch["i"].asString().c_str(), NULL);
	drone_pid.pitch_d = (double)strtod(pitch["d"].asString().c_str(), NULL);
	drone_pid.yaw_outer_p = (double)strtod(yaw["outer_p"].asString().c_str(), NULL);
	drone_pid.yaw_p = (double)strtod(yaw["p"].asString().c_str(), NULL);
	drone_pid.yaw_i = (double)strtod(yaw["i"].asString().c_str(), NULL);
	drone_pid.yaw_d = (double)strtod(yaw["d"].asString().c_str(), NULL);

	closeSettingsInstream();

	return drone_pid;
}

TRIM Config::getTrimValues(){
	TRIM drone_trim;
	drone_trim.roll = 0;
	drone_trim.pitch = 0;
	drone_trim.yaw = 0;
	if(openSettingsInstream() == -1) return drone_trim;
	string settings = "";
	while(!ifs_ds.eof()){
		string temp;
		getline(ifs_ds, temp);
		settings += temp;
	}

	Json::Value root;
	Json::Reader reader;
	if(!reader.parse(settings, root)){
		cout << "getTrimValues(): failed to parse settings." << endl;
	}

	Json::Value trim = root["trim"];
	
	drone_trim.roll = (double)strtod(trim["roll"].asString().c_str(), NULL);
	drone_trim.pitch = (double)strtod(trim["pitch"].asString().c_str(), NULL);
	drone_trim.yaw = (double)strtod(trim["yaw"].asString().c_str(), NULL);

	closeSettingsInstream();

	return drone_trim;
}


// (구)Config::getPidValues()
/*
PID Config::getPidValues(){
	PID drone_pid;
	Ahrs::initPidValues(&drone_pid);
	if(openConfigInstream() == -1) return drone_pid;
	string temp;
	while(!ifs_rc.eof()){
		getline(ifs_rc, temp);
		if(temp.find("pid=") != string::npos){
			string pid_val;
			for(int i=0; (pid_val = Util::strSplit(temp.substr(4), ",")) != ","; i++){
				switch(i){
					case 0: drone_pid.roll_inner_p = strtod(pid_val.c_str(), NULL); break;
					case 1: drone_pid.roll_p = strtod(pid_val.c_str(), NULL); break;
					case 2: drone_pid.roll_i = strtod(pid_val.c_str(), NULL); break;
					case 3: drone_pid.roll_d = strtod(pid_val.c_str(), NULL); break;
					case 4: drone_pid.pitch_inner_p = strtod(pid_val.c_str(), NULL); break;
					case 5: drone_pid.pitch_p = strtod(pid_val.c_str(), NULL); break;
					case 6: drone_pid.pitch_i = strtod(pid_val.c_str(), NULL); break;
					case 7: drone_pid.pitch_d = strtod(pid_val.c_str(), NULL); break;
					case 8: drone_pid.yaw_inner_p = strtod(pid_val.c_str(), NULL); break;
					case 9: drone_pid.yaw_p = strtod(pid_val.c_str(), NULL); break;
					case 10: drone_pid.yaw_i = strtod(pid_val.c_str(), NULL); break;
					case 11: drone_pid.yaw_d = strtod(pid_val.c_str(), NULL); break;
				}
			}
			break;
		}
	}
	closeConfigInstream();
	return drone_pid;
}
*/


int Config::setMail(string mail_value){
	if(openConfigInstream() == -1) return -1;
	string ifs_buf;
	bool value_flag = false;
	int i=0;

	string temp;
	while(!ifs_rc.eof()){
		getline(ifs_rc, temp);
		if(temp.find("email=") != string::npos){
			ifs_buf += "email=" + mail_value + "\n";
			value_flag = true;
		} else{
			ifs_buf += temp + "\n";
		}
	}
	if(value_flag == false){
		ifs_buf += "email=" + mail_value + "\n";
	}
	if(ifs_buf[ifs_buf.length()-1] == '\n') ifs_buf.erase(ifs_buf.length()-1,1);
	closeConfigInstream();	

	if(openConfigOutstream() == -1) return -1;
	ofs_rc << ifs_buf;
	closeConfigOutstream();	

	return 0;
}

int Config::setDroneNo(string dn_value){
	if(openConfigInstream() == -1) return -1;
	string ifs_buf;
	bool value_flag = false;

	string temp;
	while(!ifs_rc.eof()){
		getline(ifs_rc, temp);
		if(temp.find("drone_no=") != string::npos){
			ifs_buf += "drone_no=" + dn_value + "\n";
			value_flag = true;
		} else{
			ifs_buf += temp + "\n";
		}
	}
	if(value_flag == false){
		ifs_buf += "drone_no=" + dn_value + "\n";
	}
	if(ifs_buf[ifs_buf.length()-1] == '\n') ifs_buf.erase(ifs_buf.length()-1,1);
	closeConfigInstream();

	if(openConfigOutstream() == -1) return -1;
	ofs_rc << ifs_buf;
	closeConfigOutstream();

	return 0;
}

int Config::setSerialNumber(string sn_value){
	if(openConfigInstream() == -1) return -1;
	string ifs_buf;
	bool value_flag = false;

	string temp;
	while(!ifs_rc.eof()){
		getline(ifs_rc, temp);
		if(temp.find("serial_number=") != string::npos){
			ifs_buf += "serial_number=" + sn_value + "\n";
			value_flag = true;
		} else{
			ifs_buf += temp + "\n";
		}
	}
	if(value_flag == false){
		ifs_buf += "serial_number=" + sn_value + "\n";
	}
	if(ifs_buf[ifs_buf.length()-1] == '\n') ifs_buf.erase(ifs_buf.length()-1,1);
	closeConfigInstream();

	if(openConfigOutstream() == -1) return -1;
	ofs_rc << ifs_buf;
	closeConfigOutstream();

	return 0;
}

int Config::setDroneSettings(string set_values){
	if(openSettingsInstream() == -1) return -1;
	string ifs_buf;
	bool values_flag = false;

	string new_settings = Util::rabadonDecoder(set_values);

	string ori_settings = "";

	while(!ifs_ds.eof()){
		string temp;
		getline(ifs_ds, temp);
		ori_settings += temp;
	}

	Json::Value o_root;
	Json::Value n_root;
	Json::Reader o_reader;
	Json::Reader n_reader;

	if(!o_reader.parse(ori_settings, o_root)){
	//	cout << "failed to parse settings." << endl;
	//	return -1;
	}
	if(!n_reader.parse(new_settings, n_root)){
	//	cout << "failed to parse settings." << endl;
	//	return -1;
	}

	Json::Value o_pid = n_root["pid"];
	Json::Value o_trim = n_root["trim"];
	o_root["pid"] = o_pid;
	o_root["trim"] = o_trim;

	if(openSettingsOutstream() == -1) return -1;
	ofs_ds << o_root << endl;
	closeSettingsOutstream();

	closeSettingsInstream();

	return 0;
}

// (구)Config::setDroneSettings()
/*
int Config::setPidValues(string pid_values){
	if(openConfigInstream() == -1) return -1;
	string ifs_buf;
	bool value_flag = false;

	string temp;
	while(!ifs_rc.eof()){
		getline(ifs_rc, temp);
		if(temp.find("pid=") != string::npos){
			ifs_buf += "pid=" + pid_values + "\n";
			value_flag = true;
		} else{
			ifs_buf += temp + "\n";
		}
	}
	if(value_flag == false){
		ifs_buf += "pid=" + pid_values + "\n";
	}
	if(ifs_buf[ifs_buf.length()-1] == '\n') ifs_buf.erase(ifs_buf.length()-1,1);
	closeConfigInstream();

	if(openConfigOutstream() == -1) return -1;
	ofs_rc << ifs_buf;
	closeConfigOutstream();

	return 0;
}
*/

/*
string Util::makeHexCode(int code){
	string result = "";
	switch(code){
		case 0: result = '0'; break;
		case 1: result = '1'; break;
		case 2: result = '2'; break;
		case 3: result = '3'; break;
		case 4: result = '4'; break;
		case 5: result = '5'; break;
		case 6: result = '6'; break;
		case 7: result = '7'; break;
		case 8: result = '8'; break;
		case 9: result = '9'; break;
		case 10: result = 'A'; break;
		case 11: result = 'B'; break;
		case 12: result = 'C'; break;
		case 13: result = 'D'; break;
		case 14: result = 'E'; break;
		case 15: result = 'F'; break;
	}
	return result;
}
*/

// 개쓰래기 메서드
string Util::strSplit(string str_origin, string str_tok){
	static int pre_cut_idx = 0;
	string str_result = "";

	string str_temp = str_origin.substr(pre_cut_idx);
	int cut_idx = str_temp.find_first_of(str_tok);
	
	str_result = str_temp.substr(0, cut_idx);

	int len = str_origin.length();
	string last = str_origin.substr(str_origin.length()-1);
	if(last.compare(str_tok) != 0){
		if(len <= pre_cut_idx + str_result.length()){
			if(str_result.length() > 0){
				pre_cut_idx += str_result.length();
				return str_result;
			}
			pre_cut_idx = 0;
			return str_tok;
		}
	} else if(len <= pre_cut_idx){
		pre_cut_idx = 0;
		return str_tok;
	}
	pre_cut_idx += cut_idx + 1;

	return str_result;
}







