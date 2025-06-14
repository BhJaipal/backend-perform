package org.example;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.util.HashMap;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

public class Server {
	private HttpServer server;
	private int addr;

	public Server(int port) {
		try {
			server = HttpServer.create(new InetSocketAddress(port), 0);
		} catch (IOException e) {
			System.err.println("Server(InetSocketAddress): Cannot create server");
			System.exit(1);
		}
		addr = port;
	}

	public void add_route(String path, Handler callback) {
		server.createContext(path, new MyHandle(path, callback));
	}

	public void start() {
		server.setExecutor(null);
		System.out.println("Server started on port: http://localhost:" + addr);
		server.start();
	}
	public interface Handler {
		void handle(Request request, Response response);
	}

	static class MyHandle implements HttpHandler {
		HashMap<String, Handler> callbacks = new HashMap<>();
		public MyHandle(String path, Handler callback) {
			callbacks.put(path, callback);
		}
		@Override
		public void handle(HttpExchange exchange) throws IOException {
			try {
				for (String path : callbacks.keySet()) {
					if (exchange.getRequestURI().getPath().equalsIgnoreCase(path)) {
						InputStreamReader reader = new InputStreamReader(exchange.getRequestBody(), "UTF-8");
						BufferedReader br = new BufferedReader(reader);
						StringBuilder requestBody = new StringBuilder();
						for (String line; (line = br.readLine()) != null;) {
							requestBody.append("\n").append(line);
						}
						Request request = new Request(
								exchange.getRequestMethod(),
								exchange.getRequestURI().getPath(),
								requestBody.toString().trim(),
								exchange.getRequestHeaders().getFirst("Content-Type"));
						Response response = new Response();
						response.contentType = "text/plain";
						response.statusCode = 200;
						callbacks.get(path).handle(request, response);
						exchange.getResponseHeaders().set("Content-Type", response.contentType);
						exchange.sendResponseHeaders(response.statusCode, response.body.length());
						OutputStream os = exchange.getResponseBody();
						os.write(response.body.getBytes());
						os.close();
						return;
					}
				}
				String msg = "Not Found";
				exchange.sendResponseHeaders(404, msg.length());
				OutputStream os = exchange.getResponseBody();
				os.write(msg.getBytes());
				os.close();
			} catch (IOException e) {
				System.err.println("MyHandle.handle: Error handling request");
			}
		}
	}
}
