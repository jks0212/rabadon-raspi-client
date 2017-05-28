#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Setting.h"
#include "../json/json.h"

using namespace std;

string Setting::EMPTY_STR = "";

ifstream Setting::cIfsSetting;
ofstream Setting::cOfsSetting;

string SETTING_FILE = "setting.json";

int Setting::openSettingInstream(){
	cIfsSetting.open("setting.json");//, ios::in);
	if(cIfsSetting.is_open() == false){
		cout << "Setting->openSettingInstream(): Fail to open setting.json." << endl;
		return -1;
	}
}

int Setting::openSettingOutstream(){
	cOfsSetting.open("setting.json");//, ios::in);
	if(cOfsSetting.is_open() == false){
		cout << "Setting->openSettingOutstream(): Fail to open setting.json." << endl;
		return -1;
	}
}

void Setting::closeSettingInstream(){
	cIfsSetting.close();
}

void Setting::closeSettingOutstream(){
	cOfsSetting.close();
}


/*
Json::Value* Setting::getSettings(){
	ofstream fout(FIME_NAME);
	if(fout.is_open() == false){
		cout << "Setting->getSettings(): Fail to open setting.json." << endl;
		return NULL;
	}

	string rs;
	string temp;
	while(getline(fout, temp)){
		rs += temp;
	}
	fout.close();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "Setting->getUserId(): Fail to parse json." << endl;
		return NULL;
	}

	return root;
}
*/

string Setting::getUserId(){
	if(openSettingInstream() == -1) return EMPTY_STR;
	string rs;
	string temp;
	while(getline(cIfsSetting, temp)){
		rs += temp;
	}
	closeSettingInstream();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "Setting->getUserId(): Fail to parse json." << endl;
		return EMPTY_STR;
	}

	Json::Value jsonVal = root["user"]["id"];
	if(jsonVal == Json::nullValue){
		cout << "Setting->getUserId(): No json element." << endl;
		return EMPTY_STR;
	}

	try{
		return root["user"]["id"].asString();
	} catch(Json::LogicError const & e){
		cout << "Setting->getUserId(): " << e.what() << endl;
		return EMPTY_STR;
	}
}

string Setting::getDroneId(){
	if(openSettingInstream() == -1) return EMPTY_STR;
	string rs;
	string temp;
	while(getline(cIfsSetting, temp)){
		rs += temp;
	}
	closeSettingInstream();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "Setting->getDroneId(): Fail to parse json." << endl;
		return EMPTY_STR;
	}

	Json::Value jsonVal = root["drone"]["id"];
	if(jsonVal == Json::nullValue){
		cout << "Setting->getDroneId(): No json element." << endl;
		return EMPTY_STR;
	}

	try{
		return root["drone"]["id"].asString();
	} catch(Json::LogicError const & e){
		cout << "Setting->getDroneId(): " << e.what() << endl;
		return EMPTY_STR;
	}
}

string Setting::getSerialNumber(){
	ifstream fin(SETTING_FILE);
	if(fin.is_open() == false){
		cout << "Setting->getSerailNumber(): Fail to open setting.json." << endl;
		fin.close();
		return EMPTY_STR;
	}

//	if(openSettingInstream() == -1) return EMPTY_STR;
	string rs;
	string temp;
	while(getline(fin, temp)){
		rs += temp;
	}
	fin.close();
//	closeSettingInstream();

	Json::Reader reader;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);

	if(!isParsed){
		cout << "Setting->getSerialNumber(): Fail to parse json." << endl;
		return EMPTY_STR;
	}

	Json::Value jsonVal = root["serial"]["number"];
	if(jsonVal == Json::nullValue){
		cout << "Setting->getSerialNumber(): No json element." << endl;
		return EMPTY_STR;
	}

	try{
		return root["serial"]["number"].asString();
	} catch(Json::LogicError const & e){
		cout << "Setting->getSerialNumber(): " << e.what() << endl;
		return EMPTY_STR;
	}
}


int Setting::setSerialNumber(string serial_num){
	ifstream fin(SETTING_FILE);
	if(fin.is_open() == false){
		cout << "Setting->setSerailNumber(): Fail to open setting.json." << endl;
		return -1;
	}

//	if(openSettingInstream() == -1) return -1;
	string rs;
	string temp;
	while(getline(fin, temp)){
		rs += temp;
	}
	fin.close();
//	closeSettingInstream();

	Json::Reader reader;
	Json::StyledStreamWriter writer;
	Json::Value root;
	bool isParsed = reader.parse(rs, root);


//	Json::StyledWriter sw;
//	cout << "read : " << endl;
//	cout << sw.write(root) << endl;

/*
	if(!isParsed){
		cout << "Setting->setSerialNumber(): Fail to parse json." << endl;
		return -1;
	}
*/
	root["serial"]["number"] = serial_num;

	Json::StyledWriter styledWriter;
	string file_contents = styledWriter.write(root);

//	cout << "file : " << file_contents << endl;
	ofstream fout(SETTING_FILE);
	if(fout.is_open() == false){
		cout << "Setting->setSerailNumber(): Fail to open setting.json." << endl;
		return -1;
	}

//	if(openSettingOutstream() < 0) return -1;

//	writer.write(, root);

	fout << file_contents;
	fout.close();

//	closeSettingOutstream();

	

/*
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
*/
	return 0;
}

/*
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
*/


/*
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
*/

/*
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
*/






