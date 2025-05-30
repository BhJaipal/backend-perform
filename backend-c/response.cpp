#include <sstream>
#include <string>

#include "response.hpp"

std::string to_string(Mimetype type) {
	switch (type) {
		case Mimetype::JSON: return "application/json"; break;
		case Mimetype::HTML: return "text/html"; break;
		default: return "text/plain"; break;
	}
}

HttpResponse::HttpResponse() : body(""), type(Mimetype::TEXT) {}

std::string HttpResponse::frameHttpResponse(
	std::string statuscode, std::string statusmsg,
	std::map<std::string, std::string> headers) {
	switch (type) {
		case Mimetype::JSON:
			headers["content-type"] = "application/json";
			break;
		case Mimetype::HTML: headers["content-type"] = "text/html"; break;
		default: headers["content-type"] = "text/plain"; break;
	}
	headers["content-length"] = std::to_string(body.length());

	if (headers["method"].length()) headers["method"] = "GET";
	if (headers["path"].length()) headers["path"] = "/";
	std::ostringstream buffer;
	buffer << "HTTP/1.1 " << statuscode << " " << statusmsg << "\r\n";
	for (auto x : headers) { buffer << x.first << ": " << x.second << "\r\n"; }
	buffer << "\r\n" << body;
	return buffer.str();
}
void HttpResponse::write(std::string data) { body = data; }

Mimetype HttpResponse::get_type() { return type; }

void HttpResponse::writeJSON(Json::Value data) {
	body = data.toStyledString();
	set_mimetype(Mimetype::JSON);
}
void HttpResponse::set_mimetype(Mimetype type) { this->type = type; }