#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct sockaddr_in client;
socklen_t client_len = sizeof(struct sockaddr_in);
int server_fd = -1;
int client_fd = -1;

int socket_lua(lua_State *L) {
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	lua_pushnumber(L, server_fd);
	return 1;
}

int bind_lua(lua_State *L) {
	uint16_t port = lua_tonumber(L, -1);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	int bind_out = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));

	lua_pushnumber(L, (lua_Number)bind_out);
	return 1;
}

void accept_lua() {
	client_fd = accept(server_fd, (struct sockaddr*)&client, &client_len);
}
int listen_lua(lua_State *L) {
	int count = lua_tonumber(L, -1);
	int out = listen(server_fd, count);

	lua_getglobal(L, "Listen");
	if (!lua_isfunction(L, -1)) return 0;
	while (1) {
		accept_lua();
		lua_call(L, 0, 0);
	}
	return 1;
}

int read_lua(lua_State *L) {
	char client_req_buffer[1024];
	// handle client request.
	// read from the client.
	read(client_fd, client_req_buffer, 1024);

	std::string lines;
	for (size_t i = 0; i < 1024; i++) {
		if (client_req_buffer[i] == 0) break;
		lines += client_req_buffer[i];
	}
	std::string method, path;
	std::map<std::string, std::string> headers;

	std::size_t curr_index = 0;
	while (curr_index < lines.length()) {
		if (lines[curr_index] == ' ') break;
		method += lines[curr_index];
		curr_index++;
	}
	curr_index++;

	while (curr_index < lines.length()) {
		if (lines[curr_index] == ' ') { break; }
		path += lines[curr_index];
		curr_index++;
	}
	lines = lines.substr(lines.find("\r\n") + 2);

	std::vector<std::string> splits;
	while (!lines.empty() && lines.find("\r\n") != std::string::npos) {
		std::string line = lines.substr(0, lines.find("\r\n"));
		if (lines == "\r\n") {
			lines = "";
		} else
			lines = lines.substr(lines.find("\r\n") + 2);
		if (!line.empty())
			splits.push_back(line);
	}

	for (auto header: splits) {
		if (header.empty()) break;
		std::string k, v;
		k = header.substr(0, header.find(": "));
		v = header.substr(header.find(": ") + 2);
		headers[k] = v;
	}

	lua_createtable(L, 0, 3 + headers.size());

	lua_pushstring(L, "path");
	lua_pushstring(L, path.c_str());
	lua_settable(L, -3);

	lua_pushstring(L, "method");
	lua_pushstring(L, method.c_str());
	lua_settable(L, -3);

	lua_pushstring(L, "body");
	if (lines.empty())
		lua_pushnil(L);
	else
		lua_pushstring(L, lines.c_str());
	lua_settable(L, -3);

	for (auto pair: headers) {
		lua_pushstring(L, pair.first.c_str());
		lua_pushstring(L, pair.second.c_str());
		lua_settable(L, -3);
	}

	return 1;
}
void write_lua(lua_State *L) {
	std::string buff = lua_tostring(L, -1);
	write(client_fd, buff.c_str(), buff.size());
	close(client_fd);
}

int main(int argc, char **argv) {
	lua_State *l = luaL_newstate();
	luaL_openlibs(l);

	lua_pushcfunction(l, (lua_CFunction)socket_lua);
	lua_setglobal(l, "socket");

	lua_pushcfunction(l, (lua_CFunction)bind_lua);
	lua_setglobal(l, "bind");

	lua_pushcfunction(l, (lua_CFunction)listen_lua);
	lua_setglobal(l, "listen");

	lua_pushcfunction(l, (lua_CFunction)read_lua);
	lua_setglobal(l, "read");

	lua_pushcfunction(l, (lua_CFunction)write_lua);
	lua_setglobal(l, "write");

	luaL_dofile(l, "test.lua");
	lua_getglobal(l, "JsonParse");
	if (!lua_isfunction(l, -1)) {
		std::cout << "Ah sh\n";
		exit(1);
	}
	lua_newtable(l);
	lua_pushstring(l, "name");
	lua_pushstring(l, "jaipal");
	lua_settable(l, -3);
	lua_pushstring(l, "age");
	lua_pushnumber(l, 20);
	lua_settable(l, -3);

	lua_call(l, 1, 0);
	std::cout << lua_tostring(l, 0);
	lua_close(l);
}
