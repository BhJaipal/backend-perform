#pragma once
#include <lauxlib.h>
#include <lua.h>
#include <string>

#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

class Server {
public:
	std::string port;
	int socket_fd;
	int client_socket_fd;

	Server(int p): port(std::to_string(p)) {}
	void handleClient(lua_State *L);
	void start(lua_State *L);
	void connect_socket(lua_State *L);
};

void add_route(lua_State *L);

/**
 * @param port int
 */
void start(lua_State *L);

void write_res(lua_State *L);
