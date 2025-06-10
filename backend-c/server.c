#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json-arr.h"
#include "json-str.h"
#include "request.h"
#include "response.h"
#include "server.h"
#include <string.h>

void route_free(routeDict route) {
	str_free(route.path);
}
Array_impl(Routes, routeDict, route);

void start(Server *server);
static void handleClient(Server *server);

Server server_new(String pt) {
	Server s = {pt, 0, 0, route_array_new()};
	return s;
}


void connect_socket(Server *server) {
	server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->socket_fd < 0) {
		printf("Failed to create server socket\n");
		exit(1);
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(server->port->str));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server->socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
		0) {
		printf("Failed to bind server socket\n");
		exit(1);
	}
	if (listen(server->socket_fd, 5) < 0) {
		printf("Failed to listen on server socket\n");
		exit(1);
	}
	printf("Server started\n");
}
void add_route(Server *this, String path, callback fn) {
	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	connect_socket(this);
	client_addr_size = sizeof(struct sockaddr_in);

	while (1) {
		this->client_socket_fd = accept(this->socket_fd, (struct sockaddr*)&client_addr, &client_addr_size);
		if (this->client_socket_fd < 0) {
			printf("Failed to accept client request.\n");
			exit(1);
		}
		// Create a new thread to handle the client
		pthread_t thread;
		pthread_create(&thread, NULL, (void*(*)(void*))handleClient, this);
	}
}

// utility function to log the serving of a file.
void logServingFile(const String path, const String mimetype) {
	printf("Serving file: %s with MIME type: %s\n", path->str, mimetype->str);
}

void handleClient(Server *server) {
	char client_req_buffer[1024];
	// handle client request.
	// read from the client.
	read(server->client_socket_fd, client_req_buffer, 1024);
	HttpRequest req = http_request_new();

	parseRequest(&req, client_req_buffer);
	HttpResponse res = http_res_new();
	// write to client.
	for (unsigned i = 0; i < server->routes->len; i++) {
		routeDict x = server->routes->arr[i];
		if (!strcmp(x.path->str, req.path->str)) {
			x.fn(&req, &res);
			break;
		}
	}
	String response = frameHttpResponse(&res, "200", "OK", req.headers);
	logServingFile(req.path, to_string(get_type(&res)));

	write(server->client_socket_fd, response->str, response->len);
	close(server->client_socket_fd);
}
