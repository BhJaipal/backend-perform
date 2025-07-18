package main

import (
	"crypto"
	"encoding/json"
	"fmt"
	"math/rand"
	"net/http"
)

type User struct {
	Name string `json:"name"`
	Password string `json:"password"`
}
type UserLoggedIn struct {
	Name string `json:"name"`
	Token string `json:"token"`
}

type Message struct {
	Msg string `json:"msg"`
	Author string `json:"author"`
	Token string `json:"token"`
	Timestamp struct {
		Hour int `json:"hr"`
		Min int `json:"min"`
	} `json:"timestamp"`
}

type MessageUser struct {
	Msg string `json:"msg"`
	Author string `json:"author"`
	Timestamp struct {
		Hour int `json:"hr"`
		Min int `json:"min"`
	} `json:"timestamp"`
}

type Output struct {
	Out string `json:"output"`
}

func WriteJson(obj any, res http.ResponseWriter) {
	by, err := json.Marshal(obj)
	if err != nil {
		fmt.Printf("Can't convert to JSON")
		return
	}
	res.Write(by)
}

var chats []Message;
var users []User;

func user_new(name string, pass string) User {
	return User{ Name: name, Password: pass }
}

func sendMsg(res http.ResponseWriter,  req *http.Request) {
	var msg Message;
	json.NewDecoder(req.Body).Decode(&msg)
	does_exist := color[msg.Author]
	if does_exist == 0 {
		WriteJson(Output{"MSG_USER_404"}, res)
		return
	}
	var bs []byte
	fmt.Appendf(bs, "%d", does_exist)

	hasher := crypto.SHA256.New()
	hasher.Write(bs)

	out := hasher.Sum(nil)
	if fmt.Sprintf("%x", out) != msg.Token {
		WriteJson(Output{"MSG_USER_404"}, res)
	} else {
		chats = append(chats, msg)
		fmt.Printf("\033[38;5;%dm%s\033[0m: %s (%d:%d)\n", color[msg.Author], msg.Author,
			msg.Msg, msg.Timestamp.Hour, msg.Timestamp.Min)
		WriteJson(Output{"MSG_SENT"}, res)
	}
}

var color map[string]int

func random_num_gen() int {
	var num int = (rand.Intn(15) + 1) * 16
	return num
}

func home(res http.ResponseWriter,  req *http.Request) {
	var msg UserLoggedIn;
	json.NewDecoder(req.Body).Decode(&msg)
	does_exist := color[msg.Name]
	if does_exist == 0 {
		WriteJson(Output{"USER_404"}, res)
		return
	}
	hasher := crypto.SHA256.New()
	var bs []byte
	fmt.Appendf(bs, "%d", does_exist)
	hasher.Write(bs)
	out := hasher.Sum(nil)
	if fmt.Sprintf("%x", out) != msg.Token {
		WriteJson(Output{"USER_404"}, res)
	} else if (len(chats) != 0) {
		last := chats[len(chats) - 1]
		lastToUser := MessageUser{Msg: last.Msg, Author: last.Author, Timestamp: last.Timestamp}
		WriteJson(lastToUser, res)
	} else {
		fmt.Println("No message yet")
		WriteJson(Output{"NO_MSG"}, res)
	}
}
func login(res http.ResponseWriter,  req *http.Request) {
	var msg User;
	json.NewDecoder(req.Body).Decode(&msg);

	for i := range(len(users)) {
		if users[i].Name == msg.Name && users[i].Password == msg.Password {
			does_exist := color[msg.Name]
			hasher := crypto.SHA256.New()
			var bs []byte
			fmt.Appendf(bs, "%d", does_exist)
			hasher.Write(bs)
			out := hasher.Sum(nil)
			auth := fmt.Sprintf("%x", out)
			WriteJson(map[string]string{"auth": auth}, res)
			break
		}
	}
	WriteJson(Output{"USER_404_LOGIN"}, res)
}

func main() {
	users = []User{user_new("jaipal", "dffe86797a27a6cc1e7d4f3b7628783bc1292f310eeb352148f62a993c30c027"), user_new("hema", "hema007")};
	chats = []Message{};
	color = make(map[string]int, len(users))
	color["jaipal"] = 75;
	color["hema"] = random_num_gen();
	http.HandleFunc("/send-msg", sendMsg)
	http.HandleFunc("/", home)
	http.HandleFunc("/login", login)
	http.ListenAndServe(":8000", nil)
}
