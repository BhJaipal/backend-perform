pub mod parser;
use parser::{Table, Types};
use std::io::prelude::*;

#[cfg(test)]
mod tests {
    use super::*;

    fn write_to_db() {
        let mut db_write = std::fs::File::create("/tmp/rs-db").unwrap();
        let out = parser::dump_table(vec![
            Table::new("t1", vec![("name", Types::String), ("age", Types::U8)]),
            Table::new("t2", vec![("new", Types::U8)])
        ]);
        db_write.write_all(out.as_bytes()).unwrap();
    }
    fn read_db() -> parser::DbDataUnparsed {
        write_to_db();
        let mut db_write = std::fs::File::open("/tmp/rs-db").unwrap();
        let mut out = String::new();
        let _ = db_write.read_to_string(&mut out);
        std::fs::remove_file("/tmp/rs-db").unwrap();
        parser::parse_db(out)
    }
    #[test]
    fn check_table_names() {
        let data: Vec<Table> = read_db().tables.iter().map(|(k, v)| parser::parse_table_attrs(k.clone(), v.clone())).collect();
        let arr = [
            Table::new("t1", vec![("name", Types::String), ("age", Types::U8)]),
            Table::new("t2", vec![("new", Types::U8)])
        ];
        for table in data {
            for el in arr.iter().clone() {
                if table.name == el.name {
                    println!("{} {}", table, el);
                    assert!(table.to_string() == el.to_string());
                    break;
                }
            }
        }
    }
}
