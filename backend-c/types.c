#include "cJSON/cJSON.h"
#include "json-str.h"
#include "types.h"
#include <string.h>
#include "lib.h"

User user_new(String na, String pass, int n) {
	User u = {na, pass, n};
	u.token = str_new((char*)gen_token(n));
	return u;
}
bool user_cmp(User usr, const User other) {
	return (strcmp(usr.name->str, other.name->str) 
		&& strcmp(usr.password->str, other.password->str));
}

UserLoggedIn user_logged_new(String na, String token) {
	UserLoggedIn u = {na, token};
	return u;
}
UserLoggedIn user_logged_from_user(User usr) {
	UserLoggedIn u = {usr.name, usr.token};
	return u;
}

MessageUser msg_user_new(String t, MsgTime tm, String s) {
	MessageUser msg = {t, tm, s};
	return  msg;
}
MessageUser msg_user_from_msg(Message msg) {
	MessageUser m;
	m.text = msg.text;
	m.time.hr = msg.time.hr;
	m.time.min = msg.time.min;
	m.sender = msg.sender;
	return m;
}void msg_free(Message msg) {
	str_free(msg.sender);
	str_free(msg.text);
	str_free(msg.token);
}
void user_free(User msg) {
	str_free(msg.name);
	str_free(msg.password);
	str_free(msg.token);
}
cJSON* msg_user_to_json(MessageUser user) {
	cJSON *monitor = cJSON_CreateObject();
	cJSON_AddStringToObject(monitor, "sender", user.sender->str);
	cJSON_AddStringToObject(monitor, "text", user.text->str);
	cJSON *time = cJSON_AddObjectToObject(monitor, "time");
	cJSON_AddNumberToObject(time, "hr", user.time.hr);
	cJSON_AddNumberToObject(time, "min", user.time.min);
	return monitor;
}
