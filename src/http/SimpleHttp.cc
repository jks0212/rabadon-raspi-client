#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "SimpleHttp.h"
#include "../data/Data.h"
#include "../json/json.h"

using namespace std;

string mHeader = "\r\n";

/*
SimpleHttp::SimpleHttp(){

}
*/

int SimpleHttp::initConnection(string addr, string url, string method){
	struct sockaddr_in server_addr;

	int sock;
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		cout << "SimpleHttp->initConnection(): Fail to create socket." << endl;
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.s_addr = inet_addr(addr.c_str());
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		cout << "SimpleHttp->initConnection(): Fail to connect to the server." << endl;
		return -1;
	}

	string http_req = method + " " + url + " HTTP/1.1\r\n";
	http_req += "Host: " + addr;
	http_req += mHeader + "\r\n";

	if(send(sock, http_req.c_str(), http_req.length(), 0) < 0){
		cout << "SimpleHttp->initConnection(): Fail to send http request." << endl;
		return -1;
	}

	return sock;
}

void SimpleHttp::addHeader(string name, string contents){
	mHeader += name + ": " + contents + "\r\n";
}


Json::Value SimpleHttp::parseJsonBody(string body){
	Json::Value root;
	Json::Value empty;
	int token1 = body.find("\r\n");
	if(token1 < 0) return empty;

	string body_len_str = body.substr(0, token1);
	int body_len = (int)strtol(body_len_str.c_str(), NULL, 16);

	string body_contents = body.substr(body_len_str.length() + 2, body_len);
	string body_last = body.substr(body_len_str.length() + 2 + body_len);

	Json::Reader reader;
	bool isParsing_success = reader.parse(body_contents, root);
	if(!isParsing_success){
		return empty;
	}

	if(body_last.compare("\r\n0\r\n") != 0) return empty;
	else return root;
}


HttpResponse* SimpleHttp::checkResponse(int sock){
	char buf[2048] = {0};
	if(recv(sock, buf, sizeof(buf), 0) < 0){
		cout << "SimpleHttp->checkResponse(): Fail to receive http response." << endl;
		close(sock);
		return NULL;
	}
	close(sock);

	string res = buf;
	if(res.length() < 12){
		return NULL;
	}

	string http = res.substr(0, 9);
	if(http.compare("HTTP/1.1 ") != 0){
		return NULL;
	}

	int code = (int)strtol(res.substr(9, 3).c_str(), NULL, 10);
	if(code == 0){
		return NULL;
	}

	int start = res.find("\r\n\r\n");
	if(start < 0){
		return NULL;
	}
	start += 4;
	int end = res.length() - start - 2;
	string body = res.substr(start, end);

	HttpResponse* httpResponse = new HttpResponse();
	httpResponse->code = code;
	httpResponse->body = body;
	httpResponse->json = parseJsonBody(body);
	return httpResponse;
}

HttpResponse* SimpleHttp::get(string addr, string url){
	int sock = initConnection(addr, url, "GET");
	return checkResponse(sock);
}

HttpResponse* SimpleHttp::post(string addr, string url){
	int sock = initConnection(addr, url, "POST");
	return checkResponse(sock);
}

HttpResponse* SimpleHttp::post(string addr, string url, string req_body){
	int sock = initConnection(addr, url, "POST");
	return checkResponse(sock);
}

/*
HttpResponse* SimpleHttp::request(string addr, string url, string method){
	int sock = initConnection(addr, method, url);

	char buf[2048] = {0};
	if(recv(sock, buf, sizeof(buf), 0) < 0){
		cout << "Fail to receive http response." << endl;
		close(sock);
		return NULL;
	}
	close(sock);

	string res = buf;
	if(res.length() < 12){
		return NULL;
	}

	string http = res.substr(0, 9);
	if(http.compare("HTTP/1.1 ") != 0){
		return NULL;
	}

	int code = (int)strtol(res.substr(9, 3).c_str(), NULL, 10);
	if(code == 0){
		return NULL;
	}

	int start = res.find("\r\n\r\n");
	if(start < 0){
		return NULL;
	}
	start += 8;
	int end = res.length() - start - 6;
	string body = res.substr(start, end);

	HttpResponse* httpResponse = new HttpResponse();
	httpResponse->code = code;
	httpResponse->body = body;
	return httpResponse;

}
*/


