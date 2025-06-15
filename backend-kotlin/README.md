# Backend in Kotlin

## How to use This Server?

```kotlin
import org.example.Server

fun main() {
    val server = Server(8000)
    server.start()
    server.add_route("/", fun (req: Request, res: Response) {
        res.write("Wrong address");
        res.code = 301;
    });
    server.add_route("/auth", fun (req: Request, res: Response) {
        val map = HashMap<String, String>();
        map.put("auth", "AUTH_FAILED");
        res.writeJson(map);
        res.code = 401;
    });
    val someData = SomeData()
    server.add_route("/some-data", fun (req: Request, res: Response) {
        res.writeJson(someData);
    });
    server.start();
}
```

```kotlin
import org.example.MyJsonObject

class SomeData : MyJsonObject {
    override fun toJson() = "{...}"
}
```
