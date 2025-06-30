use actix_web::{App, HttpServer, HttpResponse, web};
use std::sync::Mutex;
mod data;
use data::*;

struct SharedState {
    users:    Mutex<Vec<User>>,
    messages: Mutex<Vec<Message>>
}
impl SharedState {
    pub const fn new() -> Self {
        SharedState {
            users: Mutex::new(Vec::new()),
            messages: Mutex::new(Vec::new())
        }
    }
}
static mut USERS: SharedState = SharedState::new();

async fn home(body: web::Json<data::MsgUser>) -> HttpResponse {
    let usr_ref = unsafe { &raw const USERS.users };
    let users = unsafe { usr_ref.read() };
    for u in users.lock().unwrap().iter().clone() {
        if u.clone().cmp_msg(body.0.clone()) {
            let msg_ref = unsafe { (&raw const USERS.messages).read() };
            if msg_ref.lock().unwrap().is_empty() {
                return HttpResponse::Ok().json(MsgStatus {output: "No messages yet".to_string()});
            }
            return HttpResponse::Ok().json(
                msg_ref.lock().unwrap().last().unwrap()
            )
        }
    }
    HttpResponse::Ok().body("Invalid token")
}

#[derive(serde::Serialize)]
struct MsgStatus {
    pub output: String
}

async fn send_msg(body: web::Json<Message>) -> HttpResponse {
    let usr_ref = unsafe { &raw const USERS.users };
    let msg_ref = unsafe { &raw const USERS.messages };
    let users = unsafe { usr_ref.read() };

    for u in users.lock().unwrap().iter().clone() {
        if u.clone().cmp_sent_msg(body.0.clone()) {
            unsafe { msg_ref.read() }.get_mut().unwrap().push(body.0.clone());
            return HttpResponse::Ok().json(MsgStatus { output: "message sent".to_string() } );
        }
    }
    HttpResponse::Ok().body("Invalid token")
}

async fn login(body: web::Json<data::LoginUser>) -> HttpResponse {
    let usr_ref = unsafe { &raw const USERS.users };
    let users = unsafe { usr_ref.read() };
    for u in users.lock().unwrap().iter().clone() {
        if u.clone().cmp_login(body.0.clone()) {
            return HttpResponse::Ok().json(u);
        }
    }
    HttpResponse::Ok().json(MsgStatus { output: "User not found".to_string() } )
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    println!("starting HTTP server at http://localhost:8000");
    // "jaipal", "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027", 75
    let usr_ref = unsafe { &raw mut USERS.users };
    let users = unsafe { usr_ref.as_mut().unwrap() };
    users.get_mut().unwrap().push(User::new(
            "jaipal".to_string(),
            "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027".to_string(),
            75));

    HttpServer::new(|| {
        App::new()
            .route("/", web::get().to(home))
            .route("/send-msg", web::get().to(send_msg))
            .route("/login", web::get().to(login))
    })
    .bind(("127.0.0.1", 8000))?
        .run().await
}
