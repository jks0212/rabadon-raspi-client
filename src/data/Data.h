#ifndef _DATA_H
#define _DATA_H

#include "../json/json.h"

namespace SIMPLEHTTP_CONST{
	enum{

	};
}

class HttpResponse{
	public:
		int code;
		std::string body;
		Json::Value json;
};

class DronePid{
	public:
		double roll_outer_p;
		double roll_p;
		double roll_i;
		double roll_d;
		double pitch_outer_p;
		double pitch_p;
		double pitch_i;
		double pitch_d;
		double yaw_outer_p;
		double yaw_p;
		double yaw_i;
		double yaw_d;
};

class DroneTrim{
	public:
		double roll;
		double pitch;
		double yaw;
};

#endif

