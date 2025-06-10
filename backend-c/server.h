#pragma once
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include "json-str.h"
#include "json-arr.h"
#include "request.h"
#include "response.h"

typedef void (*callback)(HttpRequest *, HttpResponse*);

typedef struct {
	String path;
	callback fn;
} routeDict;
Array_t(Routes, routeDict, route);

typedef struct Server {
	String port;
	int socket_fd;
	int client_socket_fd;
	Routes routes;
} Server;

static void handleClient(Server *server);
Server server_new(String pt);
void add_route(Server *this, String path, callback fn);

void connect_socket(Server *server);
void start(Server *server);
