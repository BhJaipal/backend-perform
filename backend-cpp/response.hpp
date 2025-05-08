#pragma once
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <map>
#include <string>

enum class Mimetype { JSON, HTML, TEXT };

std::string to_string(Mimetype type);

class HttpResponse {
	std::string statuscode;
	std::string statusmsg;
	std::map<std::string, std::string> headers;
	std::string body;
	Mimetype type;

public:
	HttpResponse();
	std::string frameHttpResponse(std::string statuscode, std::string statusmsg,
								  std::map<std::string, std::string> headers);

	void write(std::string data);
	void writeJSON(Json::Value data);
	void set_mimetype(Mimetype type);
	Mimetype get_type();
};