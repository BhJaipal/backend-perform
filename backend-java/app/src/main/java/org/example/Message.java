package org.example;

import java.util.HashMap;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

public class Message implements JsonObj {
	String text;
	String sender;
	String token;

	public static class MsgTime {
		int hour;
		int minute;

		public MsgTime(int hour, int minute) {
			this.hour = hour;
			this.minute = minute;
		}
	}
	MsgTime time;

	public Message(String text, String user, String token, MsgTime time) {
		this.text = text;
		this.sender = user;
		this.token = token;
		this.time = time;
	}

	public static Message readJson(String json) {
		try {
			JSONObject obj = (JSONObject) new JSONParser().parse(json);
			String text = (String) obj.get("msg");
			JSONObject time = (JSONObject) obj.get("timestamp");
			String token = (String) obj.get("token");
			String author = (String) obj.get("author");
			Message msg = new Message(text, author, token, new MsgTime(((Long) time.get("hr")).intValue(), ((Long) time.get("min")).intValue()));
			return msg;
		} catch (Exception e) {
			System.err.println("Message.readJson(String): Cannot parse json");
			System.exit(1);
		}
		return null;
	}

	public String toJson() {
		System.out.println("Here");
		if (this.time == null) {
			HashMap<String, Object> map = new HashMap<>();
			HashMap<String, Integer> time = new HashMap<>();
			time.put("hr", 0);
			time.put("min", 0);
			map.put("msg", text);
			map.put("author", sender);
			map.put("timestamp", time);

			return new JSONObject(map).toJSONString();
		}
		HashMap<String, Integer> time = new HashMap<>();
		time.put("hr", this.time.hour);
		time.put("min", this.time.minute);

		HashMap<String, Object> map = new HashMap<>();
		map.put("msg", text);
		map.put("author", sender);
		map.put("timestamp", time);
	
		JSONObject object = new JSONObject(map);
		return object.toJSONString();
	}
}
