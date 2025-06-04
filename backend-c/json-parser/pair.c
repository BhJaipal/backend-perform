#include "pair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "json-str.h"
#include "value.h"

Pair *from_i32(char *key,  int32 val) {
	Pair *p = malloc(sizeof(Pair));
	p->type = INT;
	p->key = str_new(key);
	p->value = value_new.from_i32(val);
	p->next = NULL;
	return p;
}
Pair *from_f32(char *key,  float val) {
	Pair *p = malloc(sizeof(Pair));
	p->key = str_new(key);
	p->type = FLOAT;
	p->value = value_new.from_f32(val);
	p->next = NULL;
	return p;
}
Pair *from_arr(char *key,  ValueArray arr) {
	Pair *p = malloc(sizeof(Pair));
	p->key = str_new(key);
	p->type = ARRAY;
	p->value = value_new.from_arr(arr);
	p->next = NULL;
	return p;
}
Pair *from_cstr(char *key, char *cstr) {
	Pair *p = malloc(sizeof(Pair));
	p->key = str_new(key);
	p->type = STRING;
	p->value = value_new.from_str(cstr);
	p->next = NULL;
	return p;
}
Pair *from_vals(char *key, Value *val, unsigned len) {
	Pair *p = malloc(sizeof(Pair));
	p->key = str_new(key);
	p->value = value_new.from_vals(val, len);
	p->type = ARRAY;
	p->next = NULL;
	return p;
}

Pair *from_str(char *key, String val) {
	Pair *p = malloc(sizeof(Pair));
	p->key = str_new(key);
	p->value.type = STRING;
	p->value.value.str = val;
	p->next = NULL;
	return p;
}

struct _PairBuilder PairBuilder = {
	from_i32,
	from_f32,
	from_str,
	from_arr,
	from_cstr,
	from_vals
};

void pair_print(Pair *pair, int level) {
	for (int i = 0; i < level; i++) {
		printf("\t");
	}
	printf("%s: ", pair->key->str);
	switch (pair->type) {
		case FLOAT:
			printf("%f", pair->value.value.f32);
			break;
		case ARRAY:
			value_array_print(pair->value.value.arr, level);
			break;
		case STRING:
			printf("\"%s\"", pair->value.value.str->str);
			break;
		default:
			printf("%d", pair->value.value.i32);
			break;
	}
	printf(", \n");
}


void pair_free(Pair *pair) {
	str_free(pair->key);
	value_free(pair->value);
}
