#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
//#include <mutex>
#include "Drone.h"
#include "Util.h"

using namespace std;

//mutex buf_mutex;

char Drone::control_buf[15];
char Drone::setting_buf[4096];


char* Drone::readControlBuf(){
	return this->control_buf;
}

void Drone::writeControlBuf(char buf[]){
	sprintf(control_buf, "%s", buf);
}

char* Drone::readSettingBuf(){
	return this->setting_buf;
}

void Drone::writeSettingBuf(char buf[]){
	sprintf(setting_buf, "%s", buf);
}




