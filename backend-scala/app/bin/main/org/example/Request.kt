package org.example;

class Request(path: String, method: String, contentType: String, body: String) {
	var path: String = "/";
	var method: String = "GET";
	var contentType: String =  "text/plain";
	var body: String = "";
	init {
		this.path = path
		this.method = method
		this.contentType = contentType
		this.body = body
	}
}