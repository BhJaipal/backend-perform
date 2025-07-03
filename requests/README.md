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
{ "output": "NO_MSG" }
```

### If not authenticated

```json
{ "output": "USER_404" }
```

## On /login

```json
{
  "auth": "token-generated"
}
```

### If failed

```json
{
	"output": "USER_404_LOGIN"
}
```

## On /send-msg

```json
{
	"output": "MSG_SENT"
}
```

### If not

```json
{
	"output": "MSG_USER_404"
}
```

|     Status     |        Description       |
| -------------- | ------------------------ |
|    MSG_SENT    |        Message sent      |
|     NO_MSG     |  No messages in chat yet |
|    USER_404    | You're not authenticated |
|  MSG_USER_404  |        No such user      |
| USER_404_LOGIN |        No such user      |
