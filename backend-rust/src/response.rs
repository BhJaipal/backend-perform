use std::net::TcpStream;
use std::io::{Write, BufWriter};

#[derive(Clone)]
#[repr(C)]
pub enum StatusCode {
    OK,                 // 200
    PermanentlyMoved,   // 300
    NotFound,           // 404
    NotAuthenticated    // 401
}
impl StatusCode {
    pub fn to_string(self) -> String {
        match self {
            Self::OK => "200 OK",
            Self::PermanentlyMoved => "300 Permanently Moved",
            Self::NotFound => "404 Not Found",
            _ => "401 Not Authenticated"
        }.to_string()
    }
}

#[derive(Clone, PartialEq, PartialOrd)]
pub enum ContentType {
    Plain,
    Json,
    Xml
}

impl ContentType {
    pub fn to_string(self) -> String {
        match self {
            Self::Json => "application/json",
            Self::Xml  => "application/xml",
            _          => "text/plain"
        }.to_string()
    }
}

pub struct Response {
    pub body: String,
    pub content_type: ContentType,
    pub code: StatusCode,
    stream: TcpStream
}
impl Response {
    pub fn new(stream: TcpStream) -> Self {
        Self {
            body:         String::new(),
            content_type: ContentType::Plain,
            code:         StatusCode::OK,
            stream
        }
    }
    #[allow(unused)]
    pub fn write(&mut self, data: String) {
        self.body = data;
    }
    pub fn dump(&mut self) {
        let output = self.code.clone().to_string();

        let _ = self.stream.write(format!("HTTP/1.1 {}\r\n", output).as_bytes()).unwrap();
        let _ = self.stream.write(format!("Content-Type: {}\r\n", self.content_type.clone().to_string()).as_bytes()).unwrap();
        let _ = self.stream.write(format!("\r\n{}", self.body.clone()).as_bytes()).unwrap();
        self.stream.flush().unwrap();
    }
}
