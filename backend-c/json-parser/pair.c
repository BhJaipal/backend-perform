#include "pair.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "json-arr.h"
#include "json-str.h"

Pair from_i32(String key,  int32 val) {
	Pair pair = {INT, key, {INT, 0}, NULL};
	pair.value.value.i32 = val;
	return pair;
}
Pair from_f32(String key,  float val) {
	Pair pair = {FLOAT, key, {FLOAT, 0}, NULL};
	pair.value.value.i32 = val;
	return pair;
}
Pair from_arr(String key,  ValueArray arr) {
	Pair pair = {ARRAY, key, {ARRAY, 0}, NULL};
	pair.value.value.arr = arr;
	return pair;
}
Pair from_cstr(String key, char *cstr) {
	Pair pair = {STRING, key, {STRING, 0}, NULL};
	pair.value.value.str = str_new(cstr);
	return pair;
}
Pair from_vals(String key, Value *val, unsigned len) {
	Pair pair = {ARRAY, key, {ARRAY, 0}, NULL};
	pair.value.value.arr = malloc(sizeof(struct _ValueArr));
	pair.value.value.arr->arr = val;
	pair.value.value.arr->len = len;
	return pair;
}

Pair from_str(String key, String val) {
	Pair pair = {STRING, key, {STRING, 0}, NULL};
	pair.value.value.str = val;
	return pair;
}

struct _PairBuilder PairBuilder = {
	from_i32,
	from_f32,
	from_str,
	from_arr,
	from_cstr,
	from_vals
};

void print_pair(Pair pair, int level) {
	for (int i = 0; i < level; i++) {
		printf("\t");
	}
	printf("%s: ", pair.key->str);
	switch (pair.type) {
		case FLOAT:
			printf("%f", pair.value.value.f32);
			break;
		case ARRAY:
			Value_array_print(pair.value.value.arr, level);
			break;
		case STRING:
			printf("\"%s\"", pair.value.value.str->str);
			break;
		default:
			printf("%d", pair.value.value.i32);
			break;
	}
	printf(", \n");
}

void value_print(Value value, int depth) {
	switch (value.type) {
		case FLOAT:
			printf("%f", value.value.f32);
			break;
		case ARRAY:
			Value_array_print(value.value.arr, depth);
			break;
		case STRING:
			printf("\"%s\"", value.value.str->str);
			break;
		default:
			printf("%d", value.value.i32);
			break;
	}
}
void value_free(Value val) {
	if (val.type == STRING) {
		str_free(val.value.str);
	}
}

Array_impl(Value, Value, value);

void pair_free(Pair *pair) {
	str_free(pair->key);
	value_free(pair->value);
}
