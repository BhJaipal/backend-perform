package org.example;

import java.util.HashMap;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

class MsgTime(var hour: Int, var minute: Int) {
}

class Message(var text: String, var sender: String, var token: String, var time: MsgTime) : MyJsonObject {
	override fun toJson() = "{'msg': '$text', 'author': '$sender', 'timestamp': { 'hr': ${time.hour}, 'min': ${time.minute} }}"
}

object MessageNew {
	fun create(json: String): Message? {
		try {
			val obj = (JSONObject) JSONParser().parse(json);
			val text = (String) obj.get("msg");
			val time = (JSONObject) obj.get("timestamp");
			val token = (String) obj.get("token");
			val author = (String) obj.get("author");
			val msg = Message(text, author, token, MsgTime(((Long) time.get("hr")).toInt(), ((Long) time.get("min")).toInt()));
		return msg;
	} catch (Exception e) {
		System.err.println("MessageNew.create(String): Cannot parse json");
		return null
	}
}}