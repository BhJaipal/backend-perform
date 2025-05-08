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
		res.Write([]byte("User does not exist, please login"))
		return
	}
	hasher := crypto.SHA256.New()
	var bs []byte
	fmt.Appendf(bs, "%d", does_exist)
	hasher.Write(bs)
	out := hasher.Sum(nil)
	if fmt.Sprintf("%x", out) != msg.Token {
		res.Write([]byte("Invalid member token"))
	} else {
		chats = append(chats, msg)
		fmt.Printf("\033[38;5;%dm%s\033[0m: %s (%d:%d)\n", color[msg.Author], msg.Author,
			msg.Msg, msg.Timestamp.Hour, msg.Timestamp.Min)
		res.Write([]byte("Message sent"))
	}
}

var color map[string]int

func random_num_gen() int {
	var num int = (rand.Intn(15) + 1) * 16
	return num
}
func show(res http.ResponseWriter,  req *http.Request) {
	var msg UserLoggedIn;
	json.NewDecoder(req.Body).Decode(&msg)
	does_exist := color[msg.Name]
	if does_exist == 0 {
		res.Write([]byte("User does not exist, please login"))
		return
	}
	hasher := crypto.SHA256.New()
	var bs []byte
	fmt.Appendf(bs, "%d", does_exist)
	hasher.Write(bs)
	out := hasher.Sum(nil)
	if fmt.Sprintf("%x", out) != msg.Token {
		res.Write([]byte("Invalid member token"))
	} else if (len(chats) != 0) {
		last := chats[len(chats) - 1]
		lastToUser := MessageUser{Msg: last.Msg, Author: last.Author, Timestamp: last.Timestamp}
		out, err := json.Marshal(lastToUser)
		if err != nil {
			fmt.Println("Can't send last message")
		}
		res.Write(out)
	} else {
		fmt.Println("No message yet")
		out, err := json.Marshal(map[string]string{"output": "No messages yet"})
		if err != nil {
			fmt.Println("Can't send last message")
		}
		res.Write(out)
	}
}
func auth(res http.ResponseWriter,  req *http.Request) {
	var msg User;
	json.NewDecoder(req.Body).Decode(&msg);
	auth := "AUTH_FAILED"

	for i := range(len(users)) {
		hasher := crypto.SHA256.New()
		hasher.Write([]byte(users[i].Password))
		hashedPass := hasher.Sum(nil)
		if users[i].Name == msg.Name && fmt.Sprintf("%x", hashedPass) == msg.Password {
			does_exist := color[msg.Name]
			hasher := crypto.SHA256.New()
			var bs []byte
			fmt.Appendf(bs, "%d", does_exist)
			hasher.Write(bs)
			out := hasher.Sum(nil)
			auth = fmt.Sprintf("%x", out)
			break
		}
	}
	out, err := json.Marshal(map[string]string{"auth": auth})
	if err != nil {
		fmt.Println("Can't authenticate, JSON to byte conversion error: ", auth);
	}
	res.Write(out)
}

func main() {
	users = []User{user_new("jaipal", "J1i16a12"), user_new("hema", "hema007")};
	chats = []Message{};
	color = make(map[string]int, len(users))
	for i := range(len(users)) {
		color[users[i].Name] = random_num_gen()
	}
	http.HandleFunc("/send-msg", sendMsg)
	http.HandleFunc("/", show)
	http.HandleFunc("/auth", auth)
	http.ListenAndServe(":8000", nil)
}
