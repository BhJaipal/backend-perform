#include <jsoncpp/json/value.h>

#include <string>

#include "lib.hpp"
#include "response.hpp"

class User: public JsonObj {
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
	Json::Value to_json() override {
		Json::Value val;
		val["auth"] = token;
		return val;
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

struct Message: public JsonObj {
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
	Json::Value to_json() {
		Json::Value val;
		val["msg"] = text;
		val["timestamp"]["hr"] = time.hr;
		val["timestamp"]["min"] = time.min;
		val["author"] = sender;
		return val;
	}
};
