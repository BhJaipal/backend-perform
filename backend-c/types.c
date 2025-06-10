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
}
