from http.server import BaseHTTPRequestHandler, HTTPServer
from typing import Callable, TypeAlias
from json import dumps

class Json:
    str_data_dump = {}

    def stringify(self) -> dict[str, object]: ...

HandlerCallback: TypeAlias = Callable[[BaseHTTPRequestHandler], None]

class Handler(BaseHTTPRequestHandler):
    routes: dict[str, HandlerCallback] = {}
    def __init__(self, request, client_address, server, routes: dict[str, HandlerCallback] = {}) -> None:
        self.routes = routes
        super().__init__(request, client_address, server)

    def read(self):
        lenght = self.headers["Content-Length"]
        return self.rfile.read(int(lenght) if type(lenght) == str else lenght).decode()

    def write_str(self, data: str):
        self.wfile.write(data.encode())

    def write_json(self, obj: Json):
        self.headers["Content-Type"] = "application/json"
        self.write_str(dumps(obj.stringify()))

    def do_GET(self):
        if len(self.routes) == 0:
            self.send_error(404, "Not Found")
            return
        for k in self.routes.keys():
            if k == self.path:
                self.send_response(200, "OK")
                self.routes[k](self)
                return
        self.send_error(404, "Not Found")
    def do_POST(self):
        if len(self.routes) == 0:
            self.send_error(404, "Not Found")
            return
        for k in self.routes.keys():
            if k == self.path:
                self.send_response(200, "OK")
                self.routes[k](self)
                return
        self.send_error(404, "Not Found")


class MyServer(HTTPServer):
    routes: dict[str, Callable[[Handler], None]] = {}

    def __init__(self, port: int) -> None:
        super().__init__(("", port), Handler)
        print(f"Server started at http://localhost:{port}")
        self.routes = {}
    def add_route(self, route: str, handle: Callable[[Handler], None]):
        self.routes[route] = handle

    def finish_request(self, request, client_address):
        Handler(request, client_address, self, self.routes)

