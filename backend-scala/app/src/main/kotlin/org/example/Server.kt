package org.example;

import com.sun.net.httpserver.HttpServer
import com.sun.net.httpserver.HttpHandler
import com.sun.net.httpserver.HttpExchange
import java.net.InetSocketAddress

import org.example.Request
import org.example.Response

class Handler(path: String, callback: (req: Request, res: Response) -> Unit) : HttpHandler {
	var routes = mutableMapOf<String, (req: Request, res: Response) -> Unit>();
	init {
		routes.put(path, callback);
	}
	override fun handle(exchange: HttpExchange) {
		for (route in routes) {
			if (exchange.getRequestURI().getPath() == route.key) {
				val req = Request(
					exchange.getRequestURI().getPath(),
					exchange.getRequestMethod(),
					exchange.getRequestHeaders().getFirst("Content-Type"),
					exchange.getRequestBody().toString()
				);
				val res = Response();
				route.value(req, res);
				exchange.getResponseHeaders().set("Content-Type", res.contentType);
				exchange.sendResponseHeaders(res.code, res.body.length.toLong());
				val os = exchange.getResponseBody();
				os.write(res.body.toByteArray(Charsets.UTF_8));
				os.close();
			}
		}
		val notFound = "Not Found";
		exchange.sendResponseHeaders(404, notFound.length.toLong());
		val os = exchange.getResponseBody();
		os.write(notFound.toByteArray(Charsets.UTF_8));
		os.close();
	}
}

class Server(port: Int) {
	var server: HttpServer;
	var addr: Int;
	init {
		server = HttpServer.create(InetSocketAddress(port), 0)
		addr = port;
	}
	fun add_route(path: String, handler: (req: Request, res: Response) -> Unit) {
		server.createContext(path, Handler(path, handler));
	}

	fun start() {
		server.setExecutor(null);
		print("Starting server on port http://localhost:${addr}/\n");
		server.start();
	}
}