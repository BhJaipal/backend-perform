from server import *
from json import loads

class UserInterface:
    name: str
    password: str
    token: str

class MsgInterface:
    msg: str
    author: str
    token: str
    time: dict[str, int]

class User(Json, UserInterface):
    def __init__(self, name: str, password: str, token: str):
        self.name = name
        self.password = password
        self.token = token

    def stringify(self) -> dict[str, object]:
        return {
            "name": self.name,
            "token": self.token
        }

    def from_login(obj: dict) -> UserInterface:
        return User(obj["name"], obj["password"], "")

    def from_auth(obj: dict) -> UserInterface:
        return User(obj["name"], "", obj["token"])

    def __eq__(self, value: object, /) -> bool:
        if type(value) != UserInterface:
            if (type(value) == MsgInterface):
                return value.author == self.name and value.token == self.token
            return False
        if value.password == "" or self.password == "":
            return self.name == value.name and self.token == value.token
        else:
            return self.name == value.name and self.password == value.password


class Message(Json, MsgInterface):
    def __init__(self, msg: str, author: str, token: str, hr: int, min: int):
        self.msg = msg
        self.author = author
        self.token = token
        self.time = { "hr": hr, "min": min }

    def parse(data: dict) -> MsgInterface:
        return Message(data["msg"], data["author"], data["token"], data["timestamp"]["hr"], data["timestamp"]["min"])

    def stringify(self) -> dict[str, object]:
        return {
            "msg": self.msg,
            "author": self.author,
            "timestamp": self.time
        }

class Output(Json):
    def __init__(self, out: str) -> None:
        self.out = out

    def stringify(self) -> dict[str, object]:
        return { "output": self.out }

messages: list[Message] = []
users: list[User] = []


def home(res: Handler):
    body = res.read()
    if body.strip() == "":
        res.write_json(Output("USER_404"))
        return

    user = User.from_auth(loads(body))
    for u in users:
        if u == user:
            if len(messages) == 0:
                res.write_json(Output("NO_MSG"))
                return
            res.write_json(messages[len(messages) - 1])
            return
    res.write_json(Output("USER_404"))

def login(res: Handler):
    body = res.read()
    if body.strip() == "":
        res.write_json(Output("USER_404_LOGIN"))
        return

    user = User.from_login(loads(body))
    for u in users:
        if u == user:
            res.write_json(u)
            return
    res.write_json(Output("USER_404_LOGIN"))

def send_msg(res: Handler):
    body = res.read()
    if body.strip() == "":
        res.write_json(Output("MSG_404"))
        return

    msg = Message.parse(loads(body))
    for u in users:
        if u == msg:
            messages.append(msg)
            res.write_json(Output("MSG_SENT"))
            return
    res.write_json(Output("MSG_USER_404"))

port = MyServer(8000)
port.add_route("/", home)
port.add_route("/login", login)
port.add_route("/send-msg", send_msg)
port.serve_forever()
