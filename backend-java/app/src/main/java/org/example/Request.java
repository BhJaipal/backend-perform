package org.example;

public class Request {
	String method;
	String path;
	String body;
	String contentType;
	public Request() {
		this.method = "GET"; // Default method
		this.path = "/";
		this.body = "";
		this.contentType = "application/json"; // Default content type
	}
	public Request(String method, String path, String body, String contentType) {
		this.method = method;
		this.path = path;
		this.body = body;
		this.contentType = contentType;
	}
}
