#include <lauxlib.h>
#include <lua.h>
#include <iostream>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#include "request.hpp"

std::string response;

std::map<int, std::string> codes = {
	{100, "Continue"},
	{200, "OK"},
	{201, "Created"},
	{202, "Accepted"},
	{204, "No Content"},
	// errors
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{406, "Not Acceptable"},
	{408, "Request Timeout"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{502, "Bad Gateway"},
	{503, "Service Unavailable"},
	// 300s
	{301, "Moved Permanently"},
	{302, "Found"},
	{304, "Not Modified"},
	{307, "Temporary Redirect"},
	{308, "Permanent Redirect"}
};

int write_res(lua_State *L) {
	int code = lua_tonumber(L, -1);
	std::string headers = lua_tostring(L, -2);
	std::string body = lua_tostring(L, -3);
	response = "HTTP/1.1 " + std::to_string(code) + " " + codes[code] + "\r\n";
	response += headers + "\r\n\r\n";
	response += body;
	return 0;
}

std::vector<std::string> routes;

class Server {
public:
	std::string port;
	int socket_fd;
	int client_socket_fd;

	Server(int p): port(std::to_string(p)) {}
	void handleClient(lua_State *s) {
		char client_req_buffer[1024];
		// handle client request.
		// read from the client.
		read(client_socket_fd, client_req_buffer, 1024);
		HttpRequest req;
		req.parseRequest(std::string(client_req_buffer));

		if (routes.empty()) {
			goto not_found_route;
		}
		for (auto pair : routes) {
			if (pair == req.path) {
				response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
				response += "[C]: Default response\n";
				
				std::string route = "Route";
				for (auto i: req.path) {
					if (i == '-' || i == '/')
						route += '_';
					else route += i;
				}

				lua_getglobal(s, route.c_str());
				if (!lua_isfunction(s, -1)) {
					std::cout << "\e[31m[ERROR]: \e[0m" << route << " function not found\n";
					break;
				}

				lua_newtable(s);
				lua_pushstring(s, "path");
				lua_pushstring(s, req.path.c_str());
				lua_settable(s, -3);

				lua_pushstring(s, "method");
				lua_pushstring(s, req.method.c_str());
				lua_settable(s, -3);

				std::cout << req.method << ": \e[33m'" << req.path << "' \e[32m200\e[0m\n";
				lua_call(s, 1, 0);

				goto write_response;
			}
		}
not_found_route:
		response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
		response += "'" + req.path + "' Not found\n";
		std::cout << req.method << ": \e[33m'" << req.path << "' \e[35m404\e[0m\n";

		// write to client.
write_response:
		write(client_socket_fd, response.c_str(), response.length());
		close(client_socket_fd);

		response = "";
	}
	void start(lua_State *L) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_size;
		connect_socket(L);
		client_addr_size = sizeof(struct sockaddr_in);

		while (1) {
			this->client_socket_fd = accept(this->socket_fd, (struct sockaddr*)&client_addr,
					&client_addr_size);
			if (this->client_socket_fd < 0) {
				printf("[C]: Failed to accept client request.\n");
				exit(1);
			}
			// Create a new thread to handle the client
			handleClient(L);
		}
	}
	void connect_socket(lua_State *L) {
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd < 0) {
			printf("[C]: Failed to create server socket\n");
			exit(1);
		}
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(std::stoi(port));
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
			printf("[C]: Failed to bind server socket\n");
			exit(1);
		}
		if (listen(socket_fd, 5) < 0) {
			printf("[C]: Failed to listen on server socket\n");
			exit(1);
		}
		printf("[C]: Server started\n");
	}
};

void add_route(lua_State *L) {
	std::string route = lua_tostring(L, -1);
	routes.push_back(route);
}

/**
 * @param port int
 */
void start(lua_State *L) {
	int port = lua_tonumber(L, -1);
	std::cout << "[C]: Starting server at http://localhost:" << port << "/\n";
	Server(port).start(L);
}

