# Backend in C++

## How to use this server?

```cpp
void login(HttpRequest& req, HttpResponse& res) {
    Json::Value val;
    val["auth"] = "AUTH_FAILED";
    res.writeJSON(val);
}

int main() {
    Server server = Server("8000");
    server.add_route("/", [](HttpRequest& req, HttpResponse& res) {
        res.write("Nothing");
    });
    server.add_route("/login", login);
    server.start();
    return 0;
}
```
