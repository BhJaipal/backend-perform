#pragma once
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>

#include "request.hpp"
#include "response.hpp"

class Server {
	std::string port;
	int socket_fd;
	int client_socket_fd;
	std::map<std::string, void (*)(HttpRequest&, HttpResponse&)> routes;

	void connect_socket();
	static void handleClient(Server *server);

public:
	Server(std::string pt);
	void start();
	void add_route(std::string path,
				   void (*callback)(HttpRequest&, HttpResponse&));
};
