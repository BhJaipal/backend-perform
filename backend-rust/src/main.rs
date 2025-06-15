mod server;

use std::net::TcpStream;
fn home(steam: TcpStream) {
}

fn main() {
    let mut _server = server::Server::new(8000);
    _server.add_route("/".to_string(), &(home as fn(TcpStream)));
    _server.start();
}
