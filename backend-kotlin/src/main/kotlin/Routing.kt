package com.example

import io.ktor.server.application.*
import io.ktor.server.response.*
import io.ktor.server.routing.*
import MsgSend
import MsgTime
import Message
import User
import UserLogIn
import io.ktor.server.request.receiveNullable
import UserAuth
import Output

fun Application.configureRouting() {
    var chats = mutableListOf<Message>();
    var users = mutableListOf<User>(User("jaipal", "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027", 75))

    routing {
        post("/") {
            val user = call.receiveNullable<UserAuth>()
            if (user == null) {
                call.respond("USER_404")
                return@post
            }
            for (u in users) {
                if (u.equals_auth(user)) {
                    if (chats.isEmpty()) {
                        call.respondText(text = "NO_MSG")
                    }
                    else {
                        call.respond(chats.last().send())
                    }
                    return@post
                }
            }
            call.respond("USER_404")
        }
        post("/login") {
            val user = call.receiveNullable<UserLogIn>()
            if (user == null) {
                call.respond("USER_404_LOGIN")
                return@post
            }
            for (u in users) {
                if (u.equals_login(user)) {
                    call.respond(u.send_token())
                    return@post
                }
            }
            call.respond("USER_404_LOGIN")
        }
        post("/send-msg") {
            val msg = call.receiveNullable<Message>()
            if (msg == null) {
                call.respond("MSG_404")
                return@post
            }
            for (u in users) {
                if (u.equals_msg(msg)) {
                    call.respond(Output("MSG_SENT"))
                    chats.add(msg)
                    return@post
                }
            }
            call.respond(Output("MSG_USER_404"))
        }
    }
}
