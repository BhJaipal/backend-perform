use serde::{Serialize, Deserialize, ser::SerializeMap};

#[derive(Debug, Deserialize)]
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

#[derive(Deserialize)]
pub struct Message {
    msg: String,
    author: String,
    #[allow(unused)]
    token: String,
    timestamp: MsgTime
}
impl Message {
    pub fn new(msg: String, author: String, token: String, hr: u8, min: u8) -> Self {
        Self { msg, author, token, timestamp: MsgTime {hr, min}}
    }
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
