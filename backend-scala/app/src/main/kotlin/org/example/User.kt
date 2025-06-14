package org.example;

import java.security.MessageDigest
import org.example.MyJsonObject

import org.json.simple.parser.*;
import org.json.simple.JSONObject;

class User(name: String = "", password: String = "", color: Int = 0) : MyJsonObject {
	var name: String = "";
	var password: String = "";
	var color: Int = 0;
	var token: String = "";

	init {
		this.name = name
		this.password = password
		this.color = color
		this.token = token
		val digest = MessageDigest.getInstance("SHA-256");
		val hash = digest.digest("${color}".toByteArray(Charsets.UTF_8));
		this.token = java.util.Base64.getEncoder().encodeToString(hash);
	}
	override fun toJson() = String.format("{'auth': '%s'}", token);
}
object UserNew {
	fun fromHome(json: String): User? {
		try {
			val obj = (JSONObject) JSONParser().parse(json);
			val name = (String) obj.get("name");
			val token = (String) obj.get("token");
			val user = User(name, "", 0);
			user.token = token;
			return user;
		} catch (e: Exception) {
			println("UserNew.fromHome(String): Cannot parse json")
			return null
		}
	}
	fun fromAuth(json: String): User? {
		try {
			val obj = (JSONObject) JSONParser().parse(json);
			val name = (String) obj.get("name");
			val password = (String) obj.get("password");
			val user = User(name, password, 0);
			return user;
		} catch (e: Exception) {
			println("UserNew.fromAuth(String): Cannot parse json")
			return null
		}
	}
}