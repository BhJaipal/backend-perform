#include "cJSON/cJSON.h"
#include "json-str.h"

#include "lib.h"

typedef char bool;

#ifndef false
#define false 0
#endif // !false

#ifndef true
#define true !false
#endif // !true

typedef struct {
	String name;
	String password;
	int num;
	String token;
} User;
User user_new(String na, String pass, int n);
bool user_cmp(User usr, const User other);

typedef struct UserLoggedIn {
	String name;
	String token;
} UserLoggedIn;

UserLoggedIn user_logged_new(String na, String token);
UserLoggedIn user_logged_from_user(User usr);

typedef struct MsgTime {
	char hr;
	char min;
} MsgTime;

typedef struct Message {
	String text;
	MsgTime time;
	String sender;
	String token;
} Message;

Message msg_new(String t, MsgTime tm, String s, String tok);

typedef struct MessageUser {
	String text;
	MsgTime time;
	String sender;
} MessageUser;
MessageUser msg_user_new(String t, MsgTime tm, String s);

void msg_free(Message msg);
void user_free(User msg);
cJSON* msg_user_to_json(MessageUser user);
