package org.example;

import org.json.simple.parser.*;
import org.json.simple.JSONObject;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class User implements JsonObj {
	String name;
	String password;
	int color;
	String token;

	public User() {
		name = "";
		password = "";
		color = 0;
	}

	static String[] hashStrings = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

	public User(String name, String password, int color) {
		try {
			
			this.name = name;
			this.password = password;
			this.color = color;
			MessageDigest digest = MessageDigest.getInstance("SHA-256");
			byte[] hash = digest.digest(String.valueOf(color).getBytes(StandardCharsets.UTF_8));
			token = String.valueOf(hash);
			StringBuilder hexString = new StringBuilder(2 * hash.length);
            for (int i = 0; i < hash.length; i++) {
                String hex = Integer.toHexString(0xff & hash[i]);
                if (hex.length() == 1) {
                    hexString.append('0');
                }
                hexString.append(hex);
            }
            token = hexString.toString();
		} catch (NoSuchAlgorithmException e) {
			System.err.println("User(...): Umimplemented");
		}
	}
	@Override
	public String toJson() {
		return String.format("{\"auth\":\"%s\"}", token);
	}

	public static User readJsonHome(String json) {
		try {
			JSONObject obj = (JSONObject) new JSONParser().parse(json);
			String name = (String) obj.get("name");
			String token = (String) obj.get("token");
			User user = new User(name, "", 0);
			user.token = token;
			return user;
		} catch (Exception e) {
			System.err.println("User.readJson(String): Cannot parse json");
		}
		return null;
	}
	public static User readJsonAuth(String json) {
		try {
			JSONObject obj = (JSONObject) new JSONParser().parse(json);
			String name = (String) obj.get("name");
			String password = (String) obj.get("password");
			User user = new User(name, password, 0);
			return user;
		} catch (Exception e) {
			System.err.println("User.readJson(String): Cannot parse json");
		}
		return null;
	}
}
