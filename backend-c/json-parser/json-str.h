#ifndef JSON_STR_H
#define JSON_STR_H

typedef struct _String {
	char *str;
	unsigned len;
} *String;

String str_new(char *str);
void str_append(String str, char *str2);

void str_free(String str);

#endif // !JSON_STR_H
