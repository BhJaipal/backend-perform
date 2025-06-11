#include "response.h"
#include <string.h>
#include "cJSON/cJSON.h"
#include "json-arr.h"
#include "json-str.h"
#include "request.h"

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
	res.headers = header_array_new();
	res.statuscode = str_new("");
	res.statusmsg = str_new("");
	res.type = TEXT;
	return res;
}

String frameHttpResponse(HttpResponse *res, char *statuscode, char *statusmsg,
		Headers headers) {
	String ct = str_new("content-type");
	String ct_val;
	switch (res->type) {
		case JSON:
			ct_val = str_new("application/json");
			break;
		case HTML: ct_val = str_new("text/html"); break;
		default: ct_val = str_new("text/plain"); break;
	}
	header_array_add(headers, (Header){ct, ct_val});
	header_array_add(headers, (Header){str_new("method"), str_new("GET")});
	for (unsigned i = 0; i < headers->len; i++) {
		if (!strcmp(headers->arr[i].header->str, "method") && !headers->arr[i].value->len) {
			str_cpy(headers->arr[i].value, str_new("GET"));
		}
		if (!strcmp(headers->arr[i].header->str, "path") && !headers->arr[i].value->len) {
			str_cpy(headers->arr[i].value, str_new("/"));
		}
	}

	String buffer = str_new("");
	str_append(buffer, "HTTP/1.1 ");
	str_append(buffer, statuscode);
	str_append(buffer, " ");
	str_append(buffer, statusmsg);
	str_append(buffer, "\r\n");
	for (unsigned i = 0; i < headers->len; i++) {
		Header header = headers->arr[i];
		str_append(buffer, header.header->str);
		str_append(buffer, ": ");
		str_append(buffer, header.value->str);
		str_append(buffer, "\r\n");
	}
	str_append(buffer, "\r\n");
	str_append(buffer, res->body->str);
	return buffer;
}
void res_write(HttpResponse *res, String data) {
	res->body = data;
}
void res_writeJSON(HttpResponse *res, cJSON *data) {
	str_append(res->body, cJSON_Print(data));
}
void set_mimetype(HttpResponse *res, Mimetype type) {
	res->type = type;
}
Mimetype get_type(HttpResponse *res) {
	return res->type;
}
void header_free(Header hdr) {
	str_free(hdr.header);
	str_free(hdr.value);
}
Array_impl(Headers, Header, header);
