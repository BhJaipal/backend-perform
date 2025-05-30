#include "json-str.h"
#include <stdlib.h>
#include <string.h>

String str_new(char *str) {
	String string = malloc(sizeof(struct _String));
	string->len = strlen(str);
	string->str = malloc(sizeof(char[string->len]));
	strcpy(string->str, str);
	return string;
}
void str_append(String str, char *str2) {
	str->len += strlen(str2);
	str->str = realloc(str->str, sizeof(char[str->len]));
	strcat(str->str, str2);
}

void str_free(String str) {
	if (str->str != NULL) {
		free(str->str);
	}
	free(str);
}


