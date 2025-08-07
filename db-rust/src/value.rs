use std::fmt::Display;
use std::collections::HashMap;

#[derive(PartialEq, PartialOrd)]
pub enum Types {
    U8,
    U16,
    U32,
    I8,
    I16,
    I32,
    U64,
    String,
    F32,
    F64
}

impl Display for Types {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {

        let out = match *self {
            Types::U8 => "u8",
            Types::U16 => "u16",
            Types::U32 => "u32",
            Types::I8 => "i8",
            Types::I16 => "i16",
            Types::I32 => "i32",
            Types::U64 => "u64",
            Types::String => "str",
            Types::F32 => "f32",
            Types::F64 => "f64"
        }.to_string();
        write!(f, "{}", out)
    }
}

pub struct Value {
    pub name: String,
    pub value: String
}
impl Value {
    pub fn new(name: String, value: String) -> Self {
        Value { name, value }
    }
    pub fn get(self) -> String {
        self.value.clone()
    }
    pub fn as_u8(self, r#type: Types) -> Option<u8> {
        if r#type == Types::U8 { Some(self.value.parse::<u8>().unwrap()) } else { None }
    }
    pub fn as_u16(self, r#type: Types) -> Option<u16> {
        if r#type == Types::U16 { Some(self.value.parse::<u16>().unwrap()) } else { None }
    }
    pub fn as_u32(self, r#type: Types) -> Option<u32> {
        if r#type == Types::U32 { Some(self.value.parse::<u32>().unwrap()) } else { None }
    }
    pub fn as_i8(self, r#type: Types) -> Option<i8> {
        if r#type == Types::I8 { Some(self.value.parse::<i8>().unwrap()) } else { None }
    }
    pub fn as_i16(self, r#type: Types) -> Option<i16> {
        if r#type == Types::I16 { Some(self.value.parse::<i16>().unwrap()) } else { None }
    }
    pub fn as_i32(self, r#type: Types) -> Option<i32> {
        if r#type == Types::I32 { Some(self.value.parse::<i32>().unwrap()) } else { None }
    }
    pub fn as_u64(self, r#type: Types) -> Option<u64> {
        if r#type == Types::U64 { Some(self.value.parse::<u64>().unwrap()) } else { None }
    }

    pub fn as_f32(self, r#type: Types) -> Option<f32> {
        if r#type == Types::F32 { Some(self.value.parse::<f32>().unwrap()) } else { None }
    }
    pub fn as_f64(self, r#type: Types) -> Option<f64> {
        if r#type == Types::F64 { Some(self.value.parse::<f64>().unwrap()) } else { None }
    }
}

pub struct Table {
    pub name: String,
    pub attrs: HashMap<String, Types>,
    pub rows: Vec<Vec<Value>>
}
impl Table {
    pub fn new(name: &str, attr_tuples: Vec<(&str, Types)>) -> Self {
        let mut attrs = HashMap::new();
        for (name, t) in attr_tuples {
            attrs.insert(name.to_string(), t);
        }
        Table { name: name.to_string(), attrs, rows: vec![] }
    }
}
impl Display for Table {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let out: Vec<String> = self.attrs.iter().map(|(k, v)| {
            format!("{}=> {}", k, v)
        }).collect();
        write!(f, "{} {{{}}}", self.name, out.join(","))
    }
}
