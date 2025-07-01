from server import *

class User(Json):
    def stringify(self) -> dict[str, object]:
        return {
            "name": "Jaipal"
        }


def home(res: Handler):
    print(res.read())
    res.write_json(User())

port = MyServer(8000)
port.add_route("/", home)
port.serve_forever()
