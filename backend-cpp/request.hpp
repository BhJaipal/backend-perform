#pragma once
#include <jsoncpp/json/value.h>

#include <map>
#include <string>

class HttpRequest {
public:
	std::string method;
	std::string path;
	std::map<std::string, std::string> headers;
	std::string body;
	bool hasJsonBody;

	HttpRequest();

	void parseRequest(const std::string& rawRequest);

	Json::Value getJsonBody();
};