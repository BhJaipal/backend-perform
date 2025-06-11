#pragma once
#include "json-str.h"
#include "cJSON/cJSON.h"
#include "json-arr.h"

typedef struct {
	String header, value;
} Header;

Array_t(Headers, Header, header);

typedef struct HttpRequest {
	String method;
	String path;
	Headers headers;
	String body;
	char hasJsonBody;
} HttpRequest;

HttpRequest http_request_new();
void parseRequest(HttpRequest *req, const char *rawRequest);

cJSON* getJsonBody(HttpRequest *req);
