#include "request.h"
#include <stdio.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "json-str.h"

HttpRequest http_request_new() {
	HttpRequest req = {str_new(""), str_new(""), header_array_new(), str_new(""), 0};
	return req;
}

void parseRequest(HttpRequest *req, const char *rawRequest) {
	size_t curr_index = 0;
	size_t buffer_len = strlen(rawRequest);
	while (curr_index < buffer_len) {
		if (rawRequest[curr_index] == ' ') break;
		str_push(req->method, rawRequest[curr_index]);
		curr_index++;
	}
	header_array_add(req->headers, (Header){str_new("method"), req->method});
	curr_index++;

	while (curr_index < buffer_len) {
		if (rawRequest[curr_index] == ' ') { break; }
		str_push(req->path, rawRequest[curr_index]);
		curr_index++;
	}
	header_array_add(req->headers, (Header){str_new("path"), req->path});
	while (curr_index < buffer_len && rawRequest[curr_index] != '{') {
		curr_index++;
	}
	if (curr_index < buffer_len) {
		const char *jsonString = rawRequest + curr_index;
		str_append(req->body, (char *)jsonString);
		req->hasJsonBody = 1;
	}
}
cJSON* getJsonBody(HttpRequest *req) {
	cJSON *json = cJSON_Parse(req->body->str);
	if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }
	return json;
}
