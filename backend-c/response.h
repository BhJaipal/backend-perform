#pragma once

#include "request.h"
#include "json-str.h"
#include "cJSON/cJSON.h"

typedef enum Mimetype { JSON, HTML, TEXT } Mimetype;
String to_string(Mimetype type);

typedef struct HttpResponse {
	String statuscode;
	String statusmsg;
	Headers headers;
	String body;
	Mimetype type;
} HttpResponse;

HttpResponse http_res_new();

String frameHttpResponse(HttpResponse *res, char *statuscode, char *statusmsg,
		Headers headers);

void res_write(HttpResponse *res, String data);
void res_writeJSON(HttpResponse *res, cJSON data);
void set_mimetype(HttpResponse *res, Mimetype type);
Mimetype get_type(HttpResponse *res);
