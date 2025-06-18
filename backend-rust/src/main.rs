mod server;
mod request;
mod response;
use crate::response::*;
use crate::server::Server;
use crate::request::*;

fn home(_req: Request, res: &mut Response) {
    res.body = "Hello World".to_string();
}

fn main() {
    let mut server = Server::new(8000);
    server.add_route("/".to_string(), home);
    server.start();
}
