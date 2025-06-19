use std::net::{TcpListener, SocketAddr, IpAddr, Ipv4Addr};
use std::collections::HashMap;
use crate::request::*;
use crate::response::*;

pub struct Server {
    port: u16,
    tcp_server: TcpListener,
    routes: HashMap<String, fn(Request, &mut Response)>
}
impl Server {
    pub fn new(port: u16) -> Self {
        let addr = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1)), port);
        let tcp_listener = match TcpListener::bind(addr) {
            Ok(listener) => listener,
            Err(error) => panic!("Cannot bind to Tcp Server: {error:?}")
        };
        Self {
            port,
            tcp_server: tcp_listener,
            routes: HashMap::new()
        }
    }
    pub fn add_route(&mut self, route: String, callback: fn(Request, &mut Response)) {
        self.routes.insert(route, callback);
    }
    pub fn start(&self) {
        println!("Server started at http://localhost:{}/", self.port);
        for stream in self.tcp_server.incoming() {
            match stream {
                Ok(stream) => {
                    if let Some((_k, v)) = self.routes.iter().next() {
                        let req = Request::new(stream.try_clone().unwrap());
                        let mut res = Response::new(stream);
                        v(req, &mut res);
                        res.dump();
                    }
                },
                Err(error) => {
                    panic!("Connection failed: {error:?}")
                }
            }
        }
    }
}
