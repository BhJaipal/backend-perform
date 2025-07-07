#include "./server.hpp"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Missing input lua file\n");
		exit(1);
	}
	std::string name;
	if (std::string(argv[1]) == "new") {
		if (argc < 3) {
			std::cout << "Enter server name (without .lua): ";
			std::cin >> name;
		} else {
			name = argv[2];
		}
		std::ofstream new_server(name + ".lua");
		std::string demo =
		"---@param body string\n---@param headers table<string, string>\n"
		"---@param code integer\nfunction Write(body, headers, code)\n"
		"\tif code == nil then\n\t\tcode = 200\n\tend\n\n"
		"\tlocal hasType = false\n\tfor key, value in pairs(headers) do\n"
		"\t\tif key == \"Content-Type\" then\n"
		"\t\t\thasType = true\n\t\tend\n"
		"\t\toutput = output .. key .. \": \" .. value .. \"\\r\\n\"\n\tend\n"
		"\t-- default Content-Type is text/plain\n\tif not hasType then\n"
		"\t\toutput = output .. \"Content-Type: text/plain\\r\\n\"\n"
		"\tend\n\toutput = output .. \"\\r\\n\" .. body"
		"\twrite(output, code)\nend\n\n---@param data table\n"
		"---@param headers table<string, string>\n"
		"---@param code integer\nfunction Json(data, headers, code)\n"
		"\theaders[\"Content-Type\"] = \"application/json\"\n"
		"\tWrite(data.__tostring(), headers, code)\nend"
		"---@param path string\n"
		"function Add(path)\n"
		"\tadd_route(path)end\n\n"
		"---@param port integer\n"
		"function Serve(port)\n"
		"\tstart(port)\nend\n\n"
		"-- NOTE:Type Alias for callback fn\n\n"
		"---@class Request\n"
		"---@field method string\n"
		"---@field path string\n\n"
		"-- Routes\nAdd(\"/\")\n"
		"-- _ is replaced for - and / in callback function name\n\n"
		"Add(\"/some\")\n-- NOTE:\n"
		"-- for function, replace each - and / with _ and add 'Route' at start of function declaration\n"
		"-- So, for /: Route_\n"
		"-- for /some: Route_some\n\n"
		"-- NOTE: Callback functions\n\n"
		"---@param req Request\n"
		"function Route_(req)\n"
		"\tWrite(\"[Lua]: Hello World from \" .. req.method .. req.path .. \"\\n\", {}, 200)\nend\n"
		"-- Route_some not declared so 404 Not Found\n\n"
		"-- Start server at port specified\n"
		"Serve(8080)\n";
		new_server << demo;
		new_server.close();
		exit(0);
	} else {
		name = argv[1];
		if (name.find(".lua") == std::string::npos) {
			name += ".lua";
		}
	}
	lua_State *l = luaL_newstate();
	lua_pushcfunction(l, (lua_CFunction)add_route);
	lua_setglobal(l, "add_route");

	lua_pushcfunction(l, (lua_CFunction)start);
	lua_setglobal(l, "start");

	lua_pushcfunction(l, (lua_CFunction)write_res);
	lua_setglobal(l, "write");

	luaL_dofile(l, name.c_str());
	lua_close(l);
}
