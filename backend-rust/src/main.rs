use backend_rust::{Server, Request, Response};

fn home(mut req: Request, res: &mut Response) {
    res.body = "Hello World".to_string();
    println!("{} {}", req.get_method().to_string(), req.get_path());
    for (k, v) in req.get_headers() {
        println!("{} => {}", k, v);
    }
}

fn main() {
    let mut server = Server::new(8000);
    server.add_route("/".to_string(), home);
    server.start();
}
