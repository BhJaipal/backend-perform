use actix_web::{App, HttpServer, HttpResponse, web, post};
use actix_cors::Cors;
use std::sync::{Mutex, Arc};
mod data;
use data::*;

struct SharedState {
    users: Vec<User>,
    chats: Vec<Message>
}
impl SharedState {
    pub fn new() -> Option<Arc<Mutex<Self>>> {
        Some(Arc::new(Mutex::new(SharedState {
            users: Vec::new(),
            chats: Vec::new()
        })))
    }
}
static mut DATA: Option<Arc<Mutex<SharedState>>> = None;

#[post("/")]
async fn home(body: web::Json<data::MsgUser>) -> HttpResponse {
    #[allow(static_mut_refs)]
    let usr_ref = unsafe { DATA.clone().unwrap() };
    let usr_opt = usr_ref.clone();
    let users = usr_opt.lock().unwrap().users.clone();

    for u in users.iter().clone() {
        if u.clone().cmp_msg(body.0.clone()) {
            let msg_ref = usr_ref.clone();
            let chats = msg_ref.lock().unwrap().chats.clone();

            if chats.is_empty() {
                return HttpResponse::Ok()
                    .append_header(("Access-Control-Allow-Origin", "*"))
                    .append_header(("Access-Control-Allow-Methods", "GET,POST"))
                    .json(Output::new("NO_MSG"));
            }
            return HttpResponse::Ok()
                .append_header(("Access-Control-Allow-Origin", "*"))
                .append_header(("Access-Control-Allow-Methods", "GET,POST"))
                .json(chats.last().unwrap())
        }
    }
    HttpResponse::Ok()
        .append_header(("Access-Control-Allow-Origin", "*"))
        .append_header(("Access-Control-Allow-Methods", "GET,POST"))
        .json(Output::new("USER_404"))
}

#[post("/send-msg")]
async fn send_msg(body: web::Json<Message>) -> HttpResponse {
    #[allow(static_mut_refs)]
    let usr_ref = unsafe { DATA.as_mut().unwrap() };
    let usr_opt = Arc::clone(usr_ref);
    let users = usr_opt.clone().lock().unwrap().users.clone();

    for u in users.iter().clone() {
        if u.clone().cmp_sent_msg(body.0.clone()) {
            let msg_ref = usr_ref.clone();
            let mut chats = msg_ref.lock().unwrap();
            chats.chats.push(body.0.clone());
            return HttpResponse::Ok()
                .append_header(("Access-Control-Allow-Origin", "*"))
                .append_header(("Access-Control-Allow-Methods", "GET,POST"))
                .json(Output::new("MSG_SENT"));
        }
    }
    HttpResponse::Ok()
        .append_header(("Access-Control-Allow-Origin", "*"))
        .append_header(("Access-Control-Allow-Methods", "GET,POST"))
        .body("MSG_USER_404")
}

#[post("/login")]
async fn login(body: web::Json<data::LoginUser>) -> HttpResponse {
    #[allow(static_mut_refs)]
    let usr_ref = unsafe { DATA.clone().unwrap() };
    let usr_opt = usr_ref.clone();
    let users = usr_opt.lock().unwrap().users.clone();

    for u in users.iter().clone() {
        if u.clone().cmp_login(body.0.clone()) {
            return HttpResponse::Ok()
                .append_header(("Access-Control-Allow-Origin", "*"))
                .append_header(("Access-Control-Allow-Methods", "GET,POST"))
                .json(u);
        }
    }
    HttpResponse::Ok()
        .append_header(("Access-Control-Allow-Origin", "*"))
        .append_header(("Access-Control-Allow-Methods", "GET,POST"))
        .json(Output::new("USER_404_LOGIN"))
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    println!("starting HTTP server at http://localhost:8000");
    // "jaipal", "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027", 75
    unsafe { DATA = SharedState::new() };
    #[allow(static_mut_refs)]
    let usr_ref = unsafe { DATA.as_mut().unwrap() };
    let usr_opt = Arc::clone(usr_ref);
    usr_opt.lock().unwrap().users.push(User::new(
            "jaipal".to_string(),
            "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027".to_string(),
            75));
    usr_opt.lock().unwrap().users.push(User::new(
            "hema".to_string(),
            "somepass".to_string(),
            75));


    HttpServer::new(|| {
        let cors = Cors::default().allow_any_origin();
        App::new()
            .wrap(cors)
            .service(home)
            .service(login)
            .service(send_msg)
    })
    .bind(("127.0.0.1", 8000))?
        .run().await
}
