use serde::{Serialize, Deserialize, ser::SerializeMap};

#[derive(Debug, Deserialize, Clone)]
pub struct MsgTime {
    pub hr: u8,
    pub min: u8
}
impl Serialize for MsgTime {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer {
            let mut s = serializer.serialize_map(Some(2))?;
            let _ = s.serialize_entry("hr", &self.hr);
            let _ = s.serialize_entry("min", &self.min);
            s.end()
    }
}

#[derive(Deserialize, Clone)]
pub struct Message {
    msg: String,
    author: String,
    #[allow(unused)]
    token: String,
    timestamp: MsgTime
}
impl Serialize for Message {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer {
            let mut s = serializer.serialize_map(Some(3))?;
            let _ = s.serialize_entry("msg", &self.msg);
            let _ = s.serialize_entry("author", &self.author);
            let _ = s.serialize_entry("timestamp", &self.timestamp);
            s.end()
    }
}

pub fn gen_token(i: u32) -> String {
    sha256::digest(format!("{}", i))
}
#[derive(Clone)]
pub struct User {
    name: String,
    pass: String,
    token: String,
}
impl User {
    pub fn new(name: String, pass: String, code: u32) -> User {
        User {
            name,
            pass,
            token: gen_token(code)
        }
    }
    pub fn cmp_login(self, user: LoginUser) -> bool {
        self.name == user.name && self.pass == user.password
    }
    pub fn cmp_msg(self, user: MsgUser) -> bool {
        self.name == user.name && self.token == user.token
    }
    pub fn cmp_sent_msg(self, msg: Message) -> bool {
        self.name == msg.author && self.token == msg.token
    }
}

impl Serialize for User {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer {
            let mut s = serializer.serialize_map(Some(1))?;
            let _ = s.serialize_entry("token", &self.token);
            s.end()
    }
}

#[derive(Deserialize, Clone)]
pub struct LoginUser {
    pub name: String,
    pub password: String
}
#[derive(Deserialize, Clone)]
pub struct MsgUser {
    pub name: String,
    pub token: String
}
