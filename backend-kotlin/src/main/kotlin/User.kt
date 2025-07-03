import kotlinx.serialization.Serializable

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

class User(val name: String, val password: String, code: Int) {
	lateinit var token: String
	init {
		try {
			val digest = MessageDigest.getInstance("SHA-256");
			val hash = digest.digest(("$code").toByteArray(StandardCharsets.UTF_8));
			token = hash.fold("", { str, it -> str + "%02x".format(it) })
		} catch (e: NoSuchAlgorithmException) {
			println("User(...): Umimplemented");
		}
	}
	fun equals_login(user: UserLogIn): Boolean {
		return name.lowercase() == user.name.lowercase() && password == user.password;
	}
	fun equals_msg(user: Message): Boolean {
		return name.lowercase() == user.author.lowercase() && token == user.token;
	}
	fun equals_auth(user: UserAuth): Boolean {
		return name.lowercase() == user.name.lowercase() && token == user.token;
	}
	fun send_token(): UserAuth {
		return UserAuth(name, token);
	}
}

@Serializable
data class UserLogIn(val name: String, val password: String) {}

@Serializable
data class UserAuth(val name: String, val token: String) {}

