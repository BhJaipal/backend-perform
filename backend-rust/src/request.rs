use std::collections::HashMap;
use std::net::TcpStream;
use std::io::{Read, BufReader};
use std::string::ToString;

#[derive(PartialEq, PartialOrd, Clone)]
pub enum Method {
    Get,
    Post
}

impl ToString for Method {
    fn to_string(&self) -> String {
        (if *self == Method::Post { "POST" } else { "GET" }).to_string()
    }
}

pub struct Request {
    stream_ref: TcpStream,
    had_read: bool,
    method: Method,
    path: String,
    headers: HashMap<String, String>,
    body: String
}

impl Request {
    pub fn new(stream_ref: TcpStream) -> Self {
        Self {
            stream_ref,
            had_read: false,
            method: Method::Get,
            path: String::new(),
            headers: HashMap::new(),
            body: String::new()
        }
    }
    fn start_read(&mut self) {
        self.had_read = true;
        let mut reader = BufReader::new(self.stream_ref.try_clone().unwrap());
        let mut reader_buf = [0; 500];
        let _ = reader.read_exact(&mut reader_buf);
        let mut request_str = std::str::from_utf8(reader_buf.as_slice()).unwrap().to_string();

        let _ = self.stream_ref.try_clone().unwrap().read_exact(&mut reader_buf);
        self.body = String::from_utf8(reader_buf.to_vec()).unwrap();


        let mut lines = Vec::new();
        while request_str.contains("\n") && !request_str.is_empty() {
            let (l1, l2) = request_str.split_once("\n").unwrap();
            lines.push(l1.to_string());
            request_str = l2.to_string();
        }
        let (_method, path_start) = if lines.first().unwrap().starts_with("POST") {
            (Method::Post, 4)
        } else {
            (Method::Get, 3)
        };
        let first_line = lines.first().unwrap();
        let mut path = first_line[path_start..first_line.len()].to_string();
        path = path.split_off(path.find(" ").unwrap());
        self.method = _method;
        lines = lines[1..lines.len()].to_vec();
        for line in lines {
            if line.is_empty() || !line.contains(":") { break; }
            let (k, v) = line.split_once(": ").unwrap();
            self.headers.insert(k.to_string(), v.to_string());
        }
        self.path = path;
    }
    #[allow(unused)]
    pub fn get_body(&mut self) -> String {
        if (!self.had_read) {
            self.start_read();
        }
        self.body.clone()
    }
    #[allow(unused)]
    pub fn get_method(&mut self) -> Method {
        if (!self.had_read) {
            self.start_read();
        }
        self.method.clone()
    }
    #[allow(unused)]
    pub fn get_path(&mut self) -> String {
        if (!self.had_read) {
            self.start_read();
        }
        self.path.clone()
    }
    #[allow(unused)]
    pub fn get_header(&mut self, key: String) -> Option<String> {
        if (!self.had_read) {
            self.start_read();
        }
        self.headers.clone().get(&key).cloned()
    }
    #[allow(unused)]
    pub fn get_headers(&mut self) -> HashMap<String, String> {
        if (!self.had_read) {
            self.start_read();
        }
        self.headers.clone()
    }
}
