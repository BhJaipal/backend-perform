use std::net::{TcpListener, SocketAddr, IpAddr, Ipv4Addr, TcpStream};
use std::collections::HashMap;

pub struct Server {
    tcp_server: TcpListener,
    port: u16,
    routes: HashMap<String, fn(TcpStream)>
}
impl Server {
    pub fn new(port: u16) -> Self {
        let addr = SocketAddr::new(IpAddr::V4(Ipv4Addr::new(127, 0, 0, 1)), port);
        let tcp_listener = match TcpListener::bind(addr) {
            Ok(listener) => listener,
            Err(error) => panic!("Cannot bind to Tcp Server: {error:?}")
        };
        Self {
            tcp_server: tcp_listener,
            port,
            routes: HashMap::new()
        }
    }
    pub fn add_route(&mut self, route: String, callback: &fn(TcpStream)) {
        self.routes.insert(route, *callback);
    }
    pub fn start(&self) {
        for steam in self.tcp_server.incoming() {
            match steam {
                Ok(steam) => {
                    if let Some((_k, v)) = self.routes.iter().next() {
                        v(steam);
                    }
                },
                Err(error) => {
                    panic!("Connection failed: {error:?}")
                }
            }
        }
    }
}
