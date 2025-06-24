use actix_web::{App, HttpServer, HttpResponse, web};
mod data;

async fn home() -> HttpResponse {
    HttpResponse::Ok().json(
        data::Message::new(
            "Hello World".to_string(),
            "Jaipal".to_string(),
            "token".to_string(),
            10, 26)
        )
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    println!("starting HTTP server at http://localhost:8000");

    HttpServer::new(|| {
        App::new()
            .route("/", web::get().to(home))
    })
    .bind(("127.0.0.1", 8000))?
        .run().await
}
