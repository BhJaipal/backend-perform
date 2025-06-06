#include <iostream>
#include <string>
#include <thread>

#include "request.hpp"
#include "response.hpp"
#include "server.hpp"

void Server::add_route(std::string path,
					   void (*callback)(HttpRequest&, HttpResponse&)) {
	routes[path] = callback;
}

Server::Server(std::string pt) : port(pt) {}

void Server::connect_socket() {
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		std::cerr << "Failed to create server socket\n";
		exit(1);
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(std::stoi(port));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) <
		0) {
		std::cerr << "Failed to bind server socket\n";
		exit(1);
	}
	if (listen(socket_fd, 5) < 0) {
		std::cerr << "Failed to listen on server socket\n";
		exit(1);
	}
	std::cout << "Server started\n";
}
void Server::start() {
	struct sockaddr_in client_addr;
	socklen_t client_addr_size;
	this->connect_socket();
	client_addr_size = sizeof(struct sockaddr_in);

	while (1) {
		client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_addr,
									  &client_addr_size);
		if (client_socket_fd < 0) {
			std::cerr << "Failed to accept client request." << std::endl;
			exit(1);
		}
		// Create a new thread to handle the client
		std::thread clientThread(handleClient, this);
		clientThread.detach();
	}
}

// utility function to log the serving of a file.
void logServingFile(unsigned status, const std::string method, const std::string path, const std::string& mimetype) {
	std::string esc;
	if (status < 300) {
		esc = "\e[92m";
	} else if (status < 400) {
		esc = "\e[93m";
	} else {
		esc = "\e[91m";
	}
	std::cout << esc << status << " \e[0m\e[1m" << method << "\e[0m" << " " << path << ", MIME type: " << mimetype << '\n';
}

void Server::handleClient(Server *server) {
	char client_req_buffer[1024];
	// handle client request.
	// read from the client.
	read(server->client_socket_fd, client_req_buffer, 1024);
	HttpRequest req = HttpRequest();

	req.parseRequest(client_req_buffer);
	HttpResponse res = HttpResponse();
	// write to client.
	char found = 0;
	for (auto x : server->routes) {
		if (x.first == req.path) {
			x.second(req, res);
			found = 1;
			break;
		}
	}
	std::string response;
	if (found)
		response = res.frameHttpResponse("200", "OK", req.headers);
	else
		response = res.frameHttpResponse("404", "Not Found", req.headers);
	logServingFile(found ? 200 : 404, req.method, req.path, to_string(res.get_type()));

	write(server->client_socket_fd, response.c_str(), response.length());
	close(server->client_socket_fd);
}
