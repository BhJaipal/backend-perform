#include <jsoncpp/json/value.h>

#include <string>

#include "lib.hpp"

class User {
	std::string name;
	std::string password;

public:
	int num;
	std::string token;
	User(std::string na, std::string pass, int n)
		: name(na), password(pass), num(n) {
		token = gen_token(n);
	}
	User(Json::Value val) {
		name = val["name"].asString();
		password = val["password"].asString();
	}
	std::string getName() const { return name; }
	std::string getPassword() const { return password; }

	bool operator==(const User& other) const {
		return name == other.name && password == other.password;
	}
};
class UserLoggedIn {
	std::string name;

public:
	std::string token;
	UserLoggedIn(std::string n, std::string tok) : name(n), token(tok) {}
	UserLoggedIn(Json::Value val) {
		name = val["name"].asString();
		token = val["token"].asString();
	}
	UserLoggedIn(User user) {
		name = user.getName();
		token = user.token;
	}
	std::string getName() const { return name; }
	bool operator==(const User& other) const {
		return name == other.getName() && token == other.token;
	}
};

struct MsgTime {
public:
	char hr;
	char min;
};

struct Message {
	std::string text;
	MsgTime time;
	std::string sender;
	std::string token;

	Message(std::string t, MsgTime tm, std::string s, std::string tok)
		: text(t), time(tm), sender(s), token(tok) {}

	Message(Json::Value val) {
		text = val["msg"].asString();
		time.hr = val["timestamp"]["hr"].asInt();
		time.min = val["timestamp"]["min"].asInt();
		sender = val["author"].asString();
		token = val["token"].asString();
	}
	std::string getName() const { return sender; }

	bool operator==(const User& other) const {
		return sender == other.getName() && token == other.token;
	}
};

struct MessageUser {
	std::string text;
	MsgTime time;
	std::string sender;

	MessageUser(std::string t, MsgTime tm, std::string s)
		: text(t), time(tm), sender(s) {}
	MessageUser(Message msg) {
		text = msg.text;
		time.hr = msg.time.hr;
		time.min = msg.time.min;
		sender = msg.sender;
	}

	MessageUser(Json::Value val) {
		text = val["msg"].asString();
		time.hr = val["timestamp"]["hr"].asInt();
		time.min = val["timestamp"]["min"].asInt();
		sender = val["author"].asString();
	}
	Json::Value to_json() {
		Json::Value val;
		val["msg"] = text;
		val["timestamp"]["hr"] = time.hr;
		val["timestamp"]["min"] = time.min;
		val["author"] = sender;
		return val;
	}
};
