# Backend in Java

## How to use this server?

```java
import org.example.JsonObj;

class SomeData implements JsonObj {
    // ...
    @Override
    public String toJson() {
        return String.format("{...}", some_data);
    }
}
```

```java
public class App {
    public static void main(String[] args) {
        SomeData some_data = new SomeData();
        Server server = new Server(8000);
        server.add_route("/", (request, response) -> {
            response.writeJson(some_data);
        });
        server.add_route("/auth", (request, response) -> {
            HashMap<String, String> map = new HashMap<>();
            map.put("auth", "AUTH_FAILED");
            response.setStatusCode(401);
            response.writeJson(map);
        })
        server.add_route("/some", (request, response) -> {
            response.write("Hmm.. Something");
        })
        server.start();
    }
}
```
