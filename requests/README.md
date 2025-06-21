# Expected Results

## On /

```json
{
  "msg": "Hello World",
  "author": "jaipal",
  "timestamp": {
    "hr": 20,
    "min": 21
  }
}
```
### If no messages yet

```json
{ "output": "No messages yet" }
```

## On /login

```json
{
  "auth": "token-generated"
}
```

## On /send-msg

`Message sent`
