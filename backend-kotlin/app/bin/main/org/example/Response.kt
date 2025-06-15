package org.example;

interface MyJsonObject {
	fun toJson(): String
}

class Response(code: Int = 200, contentType: String = "text/plain") {
	var code: Int
	var contentType: String
	var body: String = ""
	init {
		this.code = code
		this.contentType = contentType
	}
	fun write(body: String) {
		this.body = body
	}
	fun writeJson(Object: MyJsonObject) {
		this.body = Object.toJson()
		this.contentType = "application/json"
	}
	fun<K, V> writeJson(map: HashMap<K, V>) {
		var obj = JSONObject(map);
        code = 401;
        contentType = "application/json";
        write(obj.toJSONString());
	}
}