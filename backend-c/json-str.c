#include "json-str.h"
#include <stdlib.h>
#include <string.h>

String str_new(char *str) {
	String string = malloc(sizeof(struct _String));
	string->len = strlen(str);
	string->str = malloc(sizeof(char[string->len + 1]));
	strcpy(string->str, str);
	string->str[string->len] = 0;
	return string;
}
void str_append(String str, char *str2) {
	str->len += strlen(str2);
	str->str = realloc(str->str, sizeof(char[str->len + 1]));
	strcat(str->str, str2);
	str->str[str->len] = 0;
}
void str_push(String str, char ch) {
	str->len++;
	str->str = realloc(str->str, sizeof(char[str->len + 1]));
	str->str[str->len - 1] = ch;
	str->str[str->len] = 0;
}

void str_free(String str) {
	if (str->str != NULL) {
		free(str->str);
	}
	free(str);
}

void str_cpy(String dest, String src) {
	if (src->len == 0) {
		free(dest->str);
		dest->str = malloc(sizeof(char[1]));
		dest->str[0] = 0;
		dest->len = 0;
		return;
	} else if (src->len != dest->len) {
		dest->str = realloc(dest->str, sizeof(char[src->len + 1]));
		dest->len = src->len;
	}
	strncpy(dest->str, src->str, src->len);
	dest->str[dest->len] = 0;
}

