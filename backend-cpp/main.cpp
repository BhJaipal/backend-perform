#include <jsoncpp/json/value.h>
#include <openssl/sha.h>

#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "lib.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "types.hpp"

std::vector<User> users;
std::vector<Message> chats;
std::map<std::string, int> colors;

class Output: public JsonObj {
public:
	std::string out;
	Output(std::string o): out(o) {}
	Json::Value to_json() {
		Json::Value val;
		val["output"] = out;
		return val;
	}
};

/**
 * @brief Handles requests to access the home route.
 *
 * This function checks if the request contains a JSON body. If not, it denies
 * access to messages. If the JSON body is present, it attempts to authenticate
 * the user by checking their token. If authentication fails, access is denied.
 * If successful, and there are messages, the latest message is returned as a
 * JSON response. Otherwise, a message indicating no messages are present is
 * returned. Logs are printed to the console based on the outcome of the
 * request.
 */

void home(HttpRequest& req, HttpResponse& res) {
	JsonObj *o = new Output("USER_404");
	if (!req.hasJsonBody) {
		res.writeJSON(o);
		std::cout << "Unknown tried to access chat\n";
		delete o;
		return;
	}
	UserLoggedIn user(req.getJsonBody());
	if (user.token == "AUTH_FAILED") {
		res.writeJSON(o);
		std::cout << "\e[38;5;197m" << user.getName()
				  << " tried to access chat but didn't have token\e[0m\n";
		delete o;
		return;
	}
	for (auto usr : users) {
		if (user == usr) {
			if (chats.size()) {
				((Output *)o)->out = "NO_MSG";
				res.writeJSON(&chats[chats.size() - 1]);
			} else res.writeJSON(o);
			printf("\e[38;5;85m%s checked messages\e[0m\n",
				   user.getName().c_str());
			delete o;
			return;
		}
	}
	std::cout << "\e[38;5;197m" << user.getName()
			  << " tried to access chat but used wrong token\e[0m\n";
	res.writeJSON(o);
	delete o;
}

/**
 * @brief Handles login requests.
 *
 * If the request contains a valid JSON body, it is parsed into a User object
 * and checked against the list of registered users. If a matching user is
 * found, the user's token is returned in the "auth" key of the response JSON
 * object.
 *
 * Otherwise, the response contains the "AUTH_FAILED" string in the "auth" key.
 */
void login(HttpRequest& req, HttpResponse& res) {
	Json::Value val;
	if (req.hasJsonBody) {
		User user(req.getJsonBody());
		for (auto usr : users) {
			if (usr == user) {
				printf("\e[38;5;%dm%s\e[0m logged in\n", colors[usr.token],
					   usr.getName().c_str());
				res.writeJSON(&usr);
				return;
			}
		}
	}
	Output *o = new Output("USER_404_LOGIN");
	res.writeJSON(o);
	delete o;
}

void sendChatMsg(HttpRequest& req, HttpResponse& res) {
	Output *o = new Output("MSG_USER_404");
	if (!req.hasJsonBody) {
		res.writeJSON(o);
		delete o;
		std::cout << "Unknown tried to send message to chat\n";
		return;
	}
	Message user(req.getJsonBody());
	if (user.token == "AUTH_FAILED") {
		res.writeJSON(o);
		delete o;
		std::cout
			<< "\e[38;5;197m" << user.getName()
			<< " tried to send message to chat but didn't have token\e[0m\n";
		return;
	}
	for (auto usr : users) {
		if (user == usr) {
			o->out = "MSG_SENT";
			res.writeJSON(o);
			delete o;
			printf("\e[38;5;%dm%s\e[0m[%d:%d]: %s\n", colors[user.sender],
				   user.getName().c_str(), user.time.hr, user.time.min,
				   user.text.c_str());
			chats.push_back(user);
			return;
		}
	}
	std::cout << "\e[38;5;197m" << user.getName()
			  << " tried to access chat but used wrong token\e[0m\n";
	res.writeJSON(o);
	delete o;
}

int main() {
	// dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027
	users.push_back(
		User("jaipal",
			 "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027",
			 75));
	users.push_back(User("hema", "hema007", random_num_gen()));
	for (auto user : users) {
		colors.insert(std::make_pair(user.getName(), user.num));
	}
	chats.push_back(Message("Hello", {17, 31}, "User1", "token1"));

	// Server
	Server server = Server("8000");
	server.add_route("/", home);
	server.add_route("/login", login);
	server.add_route("/send-msg", sendChatMsg);
	server.add_route("/nothing", [](HttpRequest& req, HttpResponse& res) {
		res.write("Nothing");
	});
	server.start();
	return 0;
}
