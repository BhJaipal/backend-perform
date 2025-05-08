#pragma once
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <map>
#include <string>

#include "request.hpp"
#include "response.hpp"

class Server {
	std::string port;
	int socket_fd;
	std::map<std::string, void (*)(HttpRequest&, HttpResponse&)> routes;

public:
	Server(std::string pt);
	void connect_socket();
	void start();
	void add_route(std::string path,
				   void (*callback)(HttpRequest&, HttpResponse&));
	static void handleClient(
		int client_socket_fd,
		std::map<std::string, void (*)(HttpRequest&, HttpResponse&)> routes);
};