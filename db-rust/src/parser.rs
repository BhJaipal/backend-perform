use std::collections::HashMap;
use magic_crypt::MagicCryptTrait;
use crate::value::*;

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
        
        let db_data_section = data_split[1].to_string();
        if !db_data_section.contains("\t") {
            let name_data_pair = db_data_section.split_once("\n").unwrap();
            let data_str = name_data_pair.1.trim();
            let mut table_data_vec = Vec::new();
            if data_str.contains("\n") {
                let data: Vec<&str> = data_str.split("\n").collect();
                data.iter().for_each(|e| table_data_vec.push(e.to_string()));
            } else {
                table_data_vec = vec![data_str.to_string()];
            }
            table_data.insert(name_data_pair.0.to_string(), table_data_vec);
        } else {
            let tables_data: Vec<&str> = db_data_section.split("\t").collect();
            tables_data.iter().for_each(|db_data_section_table| {
                let name_data_pair = db_data_section_table.split_once("\n").unwrap();
                let data_str = name_data_pair.1.trim();
                let mut table_data_vec = Vec::new();
                if data_str.contains("\n") {
                    let data: Vec<&str> = data_str.split("\n").collect();
                    data.iter().for_each(|e| table_data_vec.push(e.to_string()));
                } else {
                    table_data_vec = vec![data_str.to_string()];
                }
                table_data.insert(name_data_pair.0.to_string(), table_data_vec);
            })
        }
    }
    DbDataUnparsed { tables, table_data }
}


pub fn parse_table_attrs(name: String, data: String) -> Table {
    let mut attrs = HashMap::new();
    let mcrypt = magic_crypt::new_magic_crypt!(name.clone(), 256);

    let decrypt = data;
    let decrypted_string = mcrypt.decrypt_base64_to_string(&decrypt).unwrap().trim().to_string();
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
    Table { name, attrs, rows: vec![] }
}

pub fn dump_table(data: Vec<Table>) -> String {
    let mut db_data = vec![];
    let dumped_desc: Vec<String> = data.iter().map(|table| {
        if !table.rows.is_empty() {
            let rows_str_vec: Vec<String> = table.rows.iter().map(|row| {
                let r: Vec<String> = row.iter().map(|val| val.name.clone() + ":" + &val.value.clone()).collect();
                r.join(",")
            }).collect();
            db_data.push(table.name.as_str().to_owned() + "\n" + rows_str_vec.join("\n").as_str());
        }

        let attrs: Vec<String> = table.attrs.iter().map(|(k, t)| k.clone() + ":" + &t.to_string()).collect();
        let attr_str: String = attrs.join(",");

        let mcrypt = magic_crypt::new_magic_crypt!(table.name.clone(), 256);
        let encrypted_string = mcrypt.encrypt_str_to_base64(attr_str);
        "table ".to_owned() + table.name.as_str() + " " + encrypted_string.as_str()
    }).collect();

    if db_data.is_empty() {
        dumped_desc.join("\n").to_string()
    } else {
        dumped_desc.join("\n").to_string() + "\r\n" + db_data.join("\t").as_str()
    }
}
