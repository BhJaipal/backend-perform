use std::collections::HashMap;
use std::net::TcpStream;
use std::io::{Read, BufReader};

#[derive(PartialEq, PartialOrd, Clone)]
pub enum Method {
    Get,
    Post
}

impl Method {
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
        let mut reader = BufReader::new(self.stream_ref.try_clone().unwrap());
        let mut reader_buf = Vec::new();
        let _ = reader.read_to_end(&mut reader_buf);
        let request_str = std::str::from_utf8(reader_buf.as_mut_slice()).unwrap().to_string();

        let mut body = String::new();
        let _ = self.stream_ref.try_clone().unwrap().read_to_string(&mut body);
        self.body = body;

        let (_method, path_start) = if request_str.starts_with("POST") {
            (Method::Post, 5)
        } else {
            (Method::Get, 4)
        };
        self.method = _method;
        let mut path = String::new();
        let mut path_end = false;
        let mut headers = HashMap::new();
        let mut key = String::new();
        let mut val = String::new();
        let mut is_key = true;
        let mut twine_lf = false;
        for ref mut i in path_start..request_str.len() {
            if !path_end {
                if  request_str.chars().nth(*i).unwrap().is_ascii_whitespace() {
                    path_end = true;
                    *i = request_str.find('\n').unwrap() - 1;
                } else  {
                    path.push(request_str.chars().nth(*i).unwrap());
                }
            }
            else {
                let s = request_str.chars().nth(*i).unwrap();
                match request_str.chars().nth(*i).unwrap() {
                    ':' => {
                        if !is_key { val.push(':'); }
                        else { is_key = false; }
                        twine_lf = false;
                        continue;
                    },
                    ' ' => { continue; },
                    '\n' => {
                        if twine_lf { break; }
                        headers.insert(key, val);
                        key = String::new();
                        val = String::new();
                        twine_lf = true;
                    },
                    '\r' => { continue; },
                    '\t' => { continue; },
                    _ => {
                        twine_lf = false;
                        if is_key { key.push(s); }
                        else { val.push(s); }
                    }
                }
            }
        }
        self.path = path;
        self.headers = headers;
        self.had_read = true;
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
