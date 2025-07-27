use iced::widget::{
    column, text, Column, text_input, row, button, container
};
use iced::alignment::Alignment;

#[derive(Debug, PartialEq, Clone)]
pub enum Message {
    #[allow(unused)]
    Login,
    Chat,
    ChatInput(String),
    ChatSubmit,
    NameUpdate(String),
    PassUpdate(String),
}
#[derive(Clone)]
struct Time {
    pub hr: u8,
    pub min: u8
}

#[derive(Clone)]
struct Msg {
    pub author: String,
    pub msg: String,
    pub timestamp: Time
}
impl Msg {
    pub fn new(au: &str, text: &str, hr: u8, min: u8) -> Self {
        Self {
            author: au.to_string(),
            msg: text.to_string(),
            timestamp: Time { hr, min }
        }
    }
}

#[derive(Default)]
struct Main {
    name: String,
    pass: String,
    logged_in: bool,
    chats: Vec<Msg>,
    input: String
}

fn load_msg() -> Vec<Msg> {
    vec![
        Msg::new("jaipal", "nyam", 11, 18),
        Msg::new("moe", "nyam", 11, 53),
        Msg::new("moe", "nya", 11, 53),
        Msg::new("jaipal", "moe", 11, 18),
    ]
}

fn capitalize(el: String) -> String {
    el.as_str().to_uppercase()[0..1].to_string() + &el[1..]
}
struct UserMsg<'a> {
    pub col: Column<'a, Message>,
    pub name: String
}

impl Main {
    #[allow(unused)]
    pub fn new() -> Self {
        Self {
            name: String::new(),
            pass: String::new(),
            logged_in: false,
            chats: load_msg(),
            input: String::new()
        }
    }
    pub fn update(&mut self, page: Message) {
        match page {
            Message::NameUpdate(val) => self.name = val,
            Message::PassUpdate(val) => self.pass = val,
            Message::Chat => {
                if self.name.trim().is_empty() { return; }
                if self.pass.trim().is_empty() { return; }
                self.logged_in = true
            },
            Message::Login => self.logged_in = false,
            Message::ChatInput(input) => self.input = input,
            Message::ChatSubmit => {
                if self.input.trim().is_empty() {
                    return;
                }
                let fulltime = chrono::offset::Local::now().to_rfc3339();
                let time_str = fulltime.split("T").last().unwrap();
                let time_iter = time_str.splitn(2, ":").filter(|val| val.parse::<u8>().is_ok());
                let time = time_iter.map(|el| el.parse::<u8>().unwrap()).collect::<Vec<u8>>();
                self.chats.push(Msg {
                    author: self.name.clone(),
                    msg: self.input.clone(),
                    timestamp: Time {
                        hr: *time.first().unwrap(),
                        min: *time.last().unwrap()
                    }
                });
                self.input = String::new();
            }
        }
    }
    fn user_msg<'a>(&self, last_username: String, el: Msg) -> UserMsg<'a> {
        if last_username.to_lowercase() != el.author.to_lowercase() {
            let col = column!(
                text(
                    if el.author.clone().to_uppercase() != self.name.to_uppercase() {
                        capitalize(el.author.clone().to_lowercase())
                    } else {
                        "You".to_string()
                    }
                ).style(|_| {
                    text::Style {
                        color: Some(iced::color!(0x94b7f7)),
                    }
                })
                .align_x(if el.author.clone().to_uppercase() == self.name.to_uppercase() { Alignment::End } else { Alignment::Start })
                .size(15),
                container(
                    column!(
                        text(el.msg.clone()).size(20),
                        text(format!("{}:{}{}", 
                                if el.timestamp.hr > 12 { el.timestamp.hr - 12 } else {el.timestamp.hr},
                                el.timestamp.min,
                                if el.timestamp.hr >= 12 {"PM"} else {"AM"}
                            )
                        ).size(10)
                    )
                    .padding(iced::Padding::from([10, 30]))
                )
                .style(|_| {
                    container::Style {
                        text_color: Some(iced::color!(0x94b7f7)),
                        background: Some(iced::Background::Color(iced::color!(0x151533))),
                        border: iced::Border {
                            color: iced::color!(0x5477b7),
                            width: 0.0,
                            radius: iced::border::Radius::new(2)
                        },
                        ..container::Style::default()
                    }
                })
                .width(300)
            )
            .align_x(if el.author.clone().to_uppercase() == self.name.to_uppercase() { Alignment::End } else { Alignment::Start })
            .spacing(10);
            UserMsg {
                col, name: el.author
            }
        } else {
            let col = column!(
                container(
                    column!(
                        text(el.msg.clone()).size(20),
                        text(format!("{}:{}{}", 
                                if el.timestamp.hr > 12 { el.timestamp.hr - 12 } else {el.timestamp.hr},
                                el.timestamp.min,
                                if el.timestamp.hr >= 12 {"PM"} else {"AM"}
                            )
                        ).size(10)
                    )
                    .padding(iced::Padding::from([10, 30]))
                )
                .style(|_| {
                    container::Style {
                        text_color: Some(iced::color!(0x94b7f7)),
                        background: Some(iced::Background::Color(iced::color!(0x151533))),
                        border: iced::Border {
                            color: iced::color!(0x5477b7),
                            width: 0.0,
                            radius: iced::border::Radius::new(2)
                        },
                        ..container::Style::default()
                    }
                })
                .width(300)
            )
            .align_x(if el.author.clone().to_uppercase() == self.name.to_uppercase() { Alignment::End } else { Alignment::Start })
            .spacing(10);

            UserMsg {
                col, name: el.author
            }
        }
    }
    pub fn view(&self) -> Column<'_, Message> {
        if !self.logged_in {
            column!(
                text("Login").size(35).font(iced::Font {
                    weight: iced::font::Weight::Bold,
                    ..Default::default()
                }).color(iced::color!(0x94b7f7)),
                row!(
                    text("Name: ").width(iced::Length::FillPortion(2)),
                    text_input("", &self.name).on_input(Message::NameUpdate).width(iced::Length::FillPortion(3))
                ).spacing(100).padding(iced::Padding::from([30, 200])),
                row!(
                    text("Password: ").width(iced::Length::FillPortion(2)),
                    text_input("", &self.pass).on_input(Message::PassUpdate).width(iced::Length::FillPortion(3))
                ).spacing(100).padding(iced::Padding::from([30, 200])),
                column!(button("Submit").on_press(Message::Chat)).align_x(Alignment::Center)
            ).align_x(Alignment::Center)
        } else {
            let mut last_username = String::new();
            column!(
                column(
                    self.chats.iter().map(|el| {
                        let out = self.user_msg(last_username.clone(), el.clone());
                        last_username = out.name;
                        container(out.col).padding(iced::Padding::from([0, 30]))
                            .width(2000)
                            .align_x(if self.name.to_uppercase() == el.author.clone().to_uppercase() { Alignment::End } else { Alignment::Start })
                            .into()
                    })
                )
                .padding(iced::Padding::from([20, 0]))
                .align_x(Alignment::Center)
                .height(850),
                row!(
                    text_input("Hello World", &self.input).on_input(Message::ChatInput),
                    button("send").on_press(Message::ChatSubmit)
                ).padding(iced::Padding::from([30, 50]))
            )
        }
    }

}

fn main() -> iced::Result {
    iced::application("", Main::update, Main::view).theme(|_| {
        iced::Theme::CatppuccinMocha
    })
    .run_with(|| (Main::new(), iced::Task::done(Message::Chat)))
}
