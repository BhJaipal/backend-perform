#include "value.h"
#include <stdio.h>

Value v_from_arr(ValueArray val) {
	Value v = {ARRAY, 0};
	v.value.arr = val;
	return v;
}
Value v_from_vals(Value *vals, unsigned len) {
	Value v;
	v.type = ARRAY;
	v.value.arr = malloc(sizeof(struct _ValueArray));
	v.value.arr->arr = vals;
	v.value.arr->len = len;
	return v;
}
Value v_from_str(char *str) {
	Value v = {STRING, 0};
	v.value.str = str_new(str);
	return v;
}
Value v_from_f32(float val) {
	Value v = {FLOAT, 0};
	v.value.f32 = val;
	return v;
}
Value v_from_i32(int32 val) {
	Value v = {INT, 0};
	v.value.i32 = val;
	return v;
}

struct _ValConstr value_new = {
	v_from_str,
	v_from_i32,
	v_from_f32,
	v_from_arr,
	v_from_vals
};
void value_print(Value value, int depth) {
	switch (value.type) {
		case FLOAT:
			printf("%f", value.value.f32);
			break;
		case ARRAY:
			value_array_print(value.value.arr, depth);
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

Array_impl(ValueArray, Value, value);
void value_array_print(ValueArray arr, int depth) {
	printf("[\n");
	for (size_t i = 0; i < arr->len; i++) {
		for (int j = 0; j < depth + 1; j++) {
			printf("\t");
		}
		value_print(arr->arr[i], depth + 1);
		if (i != arr->len - 1) { printf(",\n"); }
	}
	printf("\n");
	for (int j = 0; j < depth; j++) {
		printf("\t");
	}
	printf("]");
}
