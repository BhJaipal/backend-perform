package org.example;

public class Response {
	int statusCode;
	String contentType;
	String body;

	public Response() {
		statusCode = 200;
		contentType = "text/plain";
	}

	public Response(int statusCode, String contentType) {
		this.statusCode = statusCode;
		this.contentType = contentType;
	}

	void setStatusCode(int statusCode) {
		this.statusCode = statusCode;
	}

	void write(String body) {
		this.body = body;
	}

	void setContentType(String contentType) {
		this.contentType = contentType;
	}

	void writeJson(JsonObj map) {
		contentType = "application/json";
		body = map.toJson();
	}
}
