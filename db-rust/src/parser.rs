use std::collections::HashMap;
use magic_crypt::MagicCryptTrait;
use std::fmt::Display;

// table NAME encrypted_by_NAME(attr: type, ..)
// ...
// <CRLF>
// TABLE_NAME1
// data \00 data ...\n
// ...
// <TAB>
// TABLE_NAME2
// ...

pub struct DbDataUnparsed {
    pub tables: HashMap<String, String>,
    pub table_data: HashMap<String, Vec<String>>
}
impl DbDataUnparsed {

pub fn parse_tables_desc(self) -> Vec<Table> {
        self.tables.iter().map(|(k, val)| {
            parse_table_attrs(k.to_string(), val.to_string())
        }).collect()
    }
}

pub fn parse_db(mut data: String) -> DbDataUnparsed {
    data = data.trim().to_string();
    let mut tables = HashMap::new();
    let mut table_data = HashMap::new();
    if !data.contains("\r\n") {
        let table_lines: Vec<&str> = data.trim().split("\n").collect();
        let table_tuples: Vec<[String; 2]> = table_lines.iter().map(|el| {
            let table: Vec<&str> = el.trim().split(" ").collect();
            let name: String = table[1].to_string();
            let attrs: String = table[2].to_string();
            [name, attrs]
        }).collect();
        for [k, v] in table_tuples {
            tables.insert(k, v);
        }
    } else {
        let data_split: Vec<&str> = data.split("\r\n").collect();
        let table_lines: Vec<&str> = data_split[0].trim().split("\n").collect();
        let table_tuples: Vec<[String; 2]> = table_lines.iter().map(|el| {
            let table: Vec<&str> = el.trim().split(" ").collect();
            let name: String = table[1].to_string();
            let attrs: String = table[2].to_string();
            [name, attrs]
        }).collect();
        for [k, v] in table_tuples {
            tables.insert(k, v);
        }
        if data_split[1].contains("\t") {
            let table_datas: Vec<&str> = data.split("\t").collect();
            let data = table_datas.iter().map(|data| {
                let table_data_pair = data.split_once("\n");
                let name = table_data_pair.unwrap().0;
                let data_lines_str = table_data_pair.unwrap().1;
                let data_lines: Vec<&str> = data_lines_str.split("\n").collect();
                let data_string: Vec<String> = data_lines.iter().map(|e| e.to_string()).collect();
                (name.to_string(), data_string)
            });
            for el in data {
                table_data.insert(el.0, el.1);
            }
        }
    }
    DbDataUnparsed { tables, table_data }
}

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

pub struct Table {
    pub name: String,
    pub attrs: HashMap<String, Types>
}
impl Table {
    pub fn new(name: &str, attr_tuples: Vec<(&str, Types)>) -> Self {
        let mut attrs = HashMap::new();
        for (name, t) in attr_tuples {
            attrs.insert(name.to_string(), t);
        }
        Table { name: name.to_string(), attrs }
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

pub fn parse_table_attrs(name: String, data: String) -> Table {
    let mut attrs = HashMap::new();
    let mcrypt = magic_crypt::new_magic_crypt!(name.clone(), 256);

    let decrypt = data;
    let decrypted_string = mcrypt.decrypt_base64_to_string(&decrypt).unwrap().trim().to_string();
    println!("{}", decrypted_string);
    if !decrypted_string.contains(",") {
        let attr_line: Vec<&str> = decrypted_string.split(":").collect();
        let attr_n = attr_line[0].trim().to_string();
        let attr_t = match attr_line[1].trim() {
            "u8" => Types::U8,
            "u16" => Types::U16,
            "u32" => Types::U32,
            "i8" => Types::I8,
            "i16" => Types::I16,
            "i32" => Types::I32,
            "str" => Types::String,
            "f32" => Types::F32,
            "f64" => Types::F64,
            _ => Types::U64
        };
        attrs.insert(attr_n, attr_t);
    } else {
        let attrs_str: Vec<&str> = decrypted_string.split(",").collect();
        attrs_str.iter().for_each(|line| {
            let a = line.trim();
            let attr_line: Vec<&str> = a.split(":").collect();
            let attr_n = attr_line[0].trim().to_string();
            let attr_t = match attr_line[1].trim() {
                "u8" => Types::U8,
                "u16" => Types::U16,
                "u32" => Types::U32,
                "i8" => Types::I8,
                "i16" => Types::I16,
                "i32" => Types::I32,
                "str" => Types::String,
                "f32" => Types::F32,
                "f64" => Types::F64,
                _ => Types::U64
            };
            attrs.insert(attr_n, attr_t);
        });
    }
    Table { name, attrs }
}

pub fn dump_table(data: Vec<Table>) -> String {
    let dumped_desc: Vec<String> = data.iter().map(|table| {
        let attrs: Vec<String> = table.attrs.iter().map(|(k, t)| k.clone() + ":" + &t.to_string()).collect();
        let attr_str: String = attrs.join(",");
        let mcrypt = magic_crypt::new_magic_crypt!(table.name.clone(), 256);
        let encrypted_string = mcrypt.encrypt_str_to_base64(attr_str);
        "table ".to_owned() + table.name.as_str() + " " + encrypted_string.as_str()
    }).collect();
    dumped_desc.join("\n").to_string()
}
