use iced::widget::{
    column, text, Column, text_input, row, button, container
};
use iced::alignment::Alignment;

#[derive(Debug, PartialEq, Clone)]
enum Message {
    #[allow(unused)]
    Login,
    Chat,
    NameUpdate(String),
    PassUpdate(String),
}
struct Time {
    pub hr: u8,
    pub min: u8
}

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
    chats: Vec<Msg>
}

fn load_msg() -> Vec<Msg> {
    vec![
        Msg::new("jaipal", "nyam", 11, 18),
        Msg::new("moe", "nya", 11, 53)
    ]
}

impl Main {
    #[allow(unused)]
    pub fn new() -> Self {
        Self {
            name: "jaipal".to_string(),
            pass: "".to_string(),
            logged_in: true,
            chats: load_msg()
        }
    }
    pub fn update(&mut self, page: Message) {
        match page {
            Message::NameUpdate(val) => self.name = val,
            Message::PassUpdate(val) => self.pass = val,
            Message::Chat => self.logged_in = true,
            Message::Login => self.logged_in = false
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
            column(
                self.chats.iter().map(|el| {
                    container(
                        container(
                            column!(
                                text(if el.author != self.name { el.author.as_str().to_uppercase()[0..1].to_string() + &el.author[1..] } else { "You".to_string() }).size(15),
                                text(el.msg.clone()).size(20),
                                text(format!("{}:{}", el.timestamp.hr, el.timestamp.min)).size(10)
                            )
                            .padding(iced::Padding::from([10, 30]))
                        )
                        .style(|_| {
                            container::Style {
                                text_color: Some(iced::Color::BLACK),
                                background: Some(iced::Background::Color(iced::color!(0x94b7f7))),
                                border: iced::Border { radius: iced::border::Radius::new(10), ..iced::Border::default() },
                                ..container::Style::default()
                            }
                        })
                        .width(300)
                    ).padding(iced::Padding::from([20, 30]))
                        .width(1000)
                        .align_x(if self.name == el.author { Alignment::End } else { Alignment::Start })
                        .into()
                })
            ).align_x(Alignment::Center)
        }
    }

}

fn main() -> iced::Result {
    iced::application("", Main::update, Main::view).theme(|_| {
        iced::Theme::CatppuccinMacchiato
    }).run_with(|| (Main::new(), iced::Task::done(Message::Chat)))
}
