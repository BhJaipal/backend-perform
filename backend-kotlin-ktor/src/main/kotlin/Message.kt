import kotlinx.serialization.Serializable


@Serializable
data class MsgTime(val hr: Int, val min: Int) {
}

@Serializable
data class Message(val msg: String, val author: String, val timestamp: MsgTime, val token: String) {
	fun send(): MsgSend {
		return MsgSend(msg, author, timestamp)
	}
}

@Serializable
data class MsgSend(val msg: String, val author: String, val timestamp: MsgTime) {
}

@Serializable
data class Output(val output: String) {
}