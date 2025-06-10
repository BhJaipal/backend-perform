#ifndef JSON_STR_H
#define JSON_STR_H

typedef struct _String {
	char *str;
	unsigned len;
} *String;

String str_new(char *str);
void str_append(String str, char *str2);
void str_push(String str, char ch);

void str_free(String str);

void str_cpy(String dest, String src);

#endif // !JSON_STR_H

