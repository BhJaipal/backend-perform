#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>

#include <map>

#include "request.hpp"

HttpRequest::HttpRequest() : method(""), path(""), hasJsonBody(false) {}

void HttpRequest::parseRequest(const std::string& rawRequest) {
	std::size_t curr_index = 0;
	while (curr_index < rawRequest.length()) {
		if (rawRequest[curr_index] == ' ') break;
		method += rawRequest[curr_index];
		curr_index++;
	}
	headers["method"] = method;
	curr_index++;

	while (curr_index < rawRequest.length()) {
		if (rawRequest[curr_index] == ' ') { break; }
		path += rawRequest[curr_index];
		curr_index++;
	}
	headers["path"] = path;
	while (curr_index < rawRequest.length() && rawRequest[curr_index] != '{') {
		curr_index++;
	}
	if (curr_index < rawRequest.length()) {
		std::string jsonString = rawRequest.substr(curr_index);
		body = jsonString;
		hasJsonBody = true;
	}
}
Json::Value HttpRequest::getJsonBody() {
	Json::CharReaderBuilder reader;
	std::string errs;
	Json::Value jsonData;
	std::istringstream s(body);
	Json::parseFromStream(reader, s, &jsonData, &errs);
	return jsonData;
}