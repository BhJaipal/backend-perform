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
	if (!req.hasJsonBody) {
		res.write("You are not allowed to check messages without login");
		std::cout << "Unknown tried to access chat\n";
		return;
	}
	UserLoggedIn user(req.getJsonBody());
	if (user.token == "AUTH_FAILED") {
		res.write("You are not allowed to check messages without login");
		std::cout << "\e[38;5;197m" << user.getName()
				  << " tried to access chat but didn't have token\e[0m\n";
		return;
	}
	for (auto usr : users) {
		if (user == usr) {
			if (chats.size()) {
				res.writeJSON(MessageUser(chats[chats.size() - 1]).to_json());
			} else res.write("No messages yet");
			printf("\e[38;5;85m%s checked messages\e[0m\n",
				   user.getName().c_str());
			return;
		}
	}
	std::cout << "\e[38;5;197m" << user.getName()
			  << " tried to access chat but used wrong token\e[0m\n";
	res.write("Invalid user");
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
	val["auth"] = "AUTH_FAILED";
	if (req.hasJsonBody) {
		User user(req.getJsonBody());
		for (auto usr : users) {
			if (usr == user) {
				val["auth"] = usr.token;
				printf("\e[38;5;%dm%s\e[0m logged in\n", colors[usr.token],
					   usr.getName().c_str());
				break;
			}
		}
	}
	res.writeJSON(val);
}

void sendChatMsg(HttpRequest& req, HttpResponse& res) {
	if (!req.hasJsonBody) {
		res.write("You are not allowed to check messages without login");
		std::cout << "Unknown tried to senf message to chat\n";
		return;
	}
	Message user(req.getJsonBody());
	if (user.token == "AUTH_FAILED") {
		res.write("You are not allowed to check messages without login");
		std::cout
			<< "\e[38;5;197m" << user.getName()
			<< " tried to send message to chat but didn't have token\e[0m\n";
		return;
	}
	for (auto usr : users) {
		if (user == usr) {
			res.write("Message sent");
			printf("\e[38;5;%dm%s\e[0m[%d:%d]: %s\n", colors[user.sender],
				   user.getName().c_str(), user.time.hr, user.time.min,
				   user.text.c_str());
			chats.push_back(user);
			return;
		}
	}
	std::cout << "\e[38;5;197m" << user.getName()
			  << " tried to access chat but used wrong token\e[0m\n";
	res.write("Invalid user");
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
