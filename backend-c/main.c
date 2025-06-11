#include "cJSON/cJSON.h"
#include <openssl/sha.h>

#include <stdio.h>
#include <string.h>
#include "json-arr.h"
#include "json-str.h"
#include "lib.h"
#include "request.h"
#include "response.h"
#include "server.h"
#include "types.h"

Array_t(Users, User, user);
Array_t(Messages, Message, msg);
Users users;
Messages chats;

typedef struct {
	String name;
	int color;
} ColorDict;
void color_free(ColorDict c) {}
Array_t(Colors, ColorDict, color);
Colors colors;

Array_impl(Users, User, user);
Array_impl(Messages, Message, msg);
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

void home(HttpRequest *req, HttpResponse *res) {
	if (!req->hasJsonBody) {
		res_write(res, str_new("You are not allowed to check messages without login"));
		printf("Unknown tried to access chat\n");
		return;
	}
	cJSON *parsed = getJsonBody(req);
	UserLoggedIn user = user_logged_new(
		str_new(cJSON_GetObjectItemCaseSensitive(parsed, "name")->valuestring),
		str_new(cJSON_GetObjectItemCaseSensitive(parsed, "token")->valuestring)
	);
	if (!strcmp(user.token->str, "AUTH_FAILED")) {
		res_write(res, str_new("You are not allowed to check messages without login"));
		printf("\e[38;5;197m%s tried to access chat but didn't have token\e[0m\n", user.name->str);
		return;
	}
	for (unsigned i = 0; i < users->len; i++) {
		User usr = users->arr[i];
		if (!strcmp(usr.name->str, user.name->str) && !strcmp(usr.token->str, user.token->str)) {
			if (chats->len) {
				Message msg = chats->arr[chats->len - 1];
				res_writeJSON(res, msg_user_to_json(
					msg_user_new(msg.sender, msg.time, msg.text)
				));
			} else res_write(res, str_new("No messages yet"));
			printf("\e[38;5;85m%s checked messages\e[0m\n", user.name->str);
			return;
		}
	}
	printf("\e[38;5;197m%s tried to access chat but used wrong token\e[0m\n", user.name->str);
	res_write(res, str_new("Invalid user"));
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
void login(HttpRequest *req, HttpResponse *res) {
	cJSON *val = cJSON_CreateObject();
	if (req->hasJsonBody) {
		cJSON *parsed = getJsonBody(req);
		User user = {
			str_new(cJSON_GetObjectItemCaseSensitive(parsed, "name")->valuestring),
			str_new(cJSON_GetObjectItemCaseSensitive(parsed, "password")->valuestring)
		};
		for (unsigned i = 0; i < users->len; i++) {
			User usr = users->arr[i];
			if (user_cmp(usr, user)) {
				cJSON_AddStringToObject(val, "auth", usr.token->str);
				printf("\e[38;5;%dm%s\e[0m logged in\n", colors->arr[i].color, usr.name->str);
				break;
			}
		}
	} else 
		cJSON_AddStringToObject(val, "auth", "AUTH_FAILED");
	res_writeJSON(res, val);
}

void sendChatMsg(HttpRequest *req, HttpResponse *res) {
	if (!req->hasJsonBody) {
		res_write(res, str_new("You are not allowed to check messages without login"));
		printf("Unknown tried to senf message to chat\n");
		return;
	}
	cJSON *parsed = getJsonBody(req);
	Message user = msg_new(
		str_new(cJSON_GetObjectItemCaseSensitive(parsed, "sender")->valuestring),
		(MsgTime){
			cJSON_GetObjectItemCaseSensitive(
				cJSON_GetObjectItemCaseSensitive(parsed, "time")->child, "hr"
			)->valueint,
			cJSON_GetObjectItemCaseSensitive(
				cJSON_GetObjectItemCaseSensitive(parsed, "time")->child, "min"
			)->valueint
		},
		str_new(cJSON_GetObjectItemCaseSensitive(parsed, "text")->valuestring),
		str_new(cJSON_GetObjectItemCaseSensitive(parsed, "token")->valuestring)
	);
	if (!strcmp(user.token->str, "AUTH_FAILED")) {
		res_write(res, str_new("You are not allowed to check messages without login"));
		printf("\e[38;5;197m%s tried to send message to chat but didn't have token\e[0m\n", user.sender->str);
		return;
	}
	for (unsigned i =0; i< users->len; i++) {
		User usr = users->arr[i];
		if (!strcmp(user.sender->str, usr.name->str) && !strcmp(user.token->str, usr.token->str)) {
			res_write(res, str_new("Message sent"));
			printf("\e[38;5;%dm%s\e[0m[%d:%d]: %s\n", colors->arr[i].color,
				   user.sender->str, user.time.hr, user.time.min,
				   user.text->str);
			msg_array_add(chats, user);
			return;
		}
	}
	printf("\e[38;5;197m%s tried to access chat but used wrong token\e[0m\n", user.sender->str);
	res_write(res, str_new("Invalid user"));
}

int main() {
	users = user_array_new();
	chats = msg_array_new();
	// dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027
	user_array_add(users, user_new(str_new("jaipal"), str_new("dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027"), 75));
	user_array_add(users, user_new(str_new("hema"), str_new("hema007"), random_num_gen()));
	for (unsigned i = 0; i < users->len; i++) {
	   User user = users->arr[i];
	   color_array_add(colors, (ColorDict){user.name, 75});
	}
	msg_array_add(chats, msg_new(str_new("Hello"), (MsgTime){17, 31}, str_new("User1"), str_new("token1")));

	// Server
	Server server = server_new(str_new("8000"));
	add_route(&server, str_new("/"), home);
	add_route(&server, str_new("/login"), login);
	add_route(&server, str_new("/send-msg"), sendChatMsg);
	start(&server);
	return 0;
}
