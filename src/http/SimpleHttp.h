#ifndef _SIMPLEHTTP_H
#define _SIMPLEHTTP_H

#include "../data/Data.h"
#include "../json/json.h"

namespace SIMPLEHTTP_CONST{
	enum{

	};
}

class SimpleHttp{
	private:
		int initConnection(std::string addr, std::string url, std::string method);
		HttpResponse* checkResponse(int sock);
	//	std::string parseBody(std::string body);
		Json::Value parseJsonBody(std::string body);
	public:
	//	SimpleHttp();

		void addHeader(std::string name, std::string contents);
	//	HttpResponse* request(std::string addr, std::string url, std::string method);
		HttpResponse* get(std::string addr, std::string url);
		HttpResponse* post(std::string addr, std::string url);
		HttpResponse* post(std::string addr, std::string url, std::string body);
};

#endif

