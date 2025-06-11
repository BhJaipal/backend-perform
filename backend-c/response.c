#include "response.h"
#include "json-str.h"

String to_string(Mimetype type) {
	switch (type) {
		case JSON: return str_new("application/json"); break;
		case HTML: return str_new("text/html"); break;
		default: return str_new("text/plain"); break;
	}
}

HttpResponse http_res_new() {
	HttpResponse res;
	res.body = str_new("");
	res.type = TEXT;
	return res;
}

String frameHttpResponse(HttpResponse *res, char *statuscode, char *statusmsg,
		Headers headers) {
	switch (type) {
		case JSON:
			headers["content-type"] = "application/json";
			break;
		case HTML: headers["content-type"] = "text/html"; break;
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
