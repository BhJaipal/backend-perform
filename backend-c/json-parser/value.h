#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include "json-arr.h"
#include "json-str.h"

#ifdef __x86_64
typedef int int32;
#else
typedef long int32;
#endif

typedef enum {
	INT,
	FLOAT,
	STRING,
	ARRAY
} JsonTypes;

struct Value;

typedef struct _ValueArray {
	struct Value *arr;
	unsigned len;
} *ValueArray;

typedef struct Value {
	JsonTypes  type;
	union {
		int32      i32;
		float      f32;
		String     str;
		ValueArray arr;
	} value;
} Value;


struct _ValConstr {
	Value (*from_str)(char *str);
	Value (*from_i32)(int32 val);
	Value (*from_f32)(float val);
	Value (*from_arr)(ValueArray arr);
	Value (*from_vals)(Value *val, unsigned len);
};

extern struct _ValConstr value_new;

void value_free(Value val);
void value_array_print(ValueArray arr, int depth);
void value_array_add(ValueArray arr, Value el);
void value_array_free (ValueArray array);
ValueArray value_array_new();

#endif // !JSON_VALUE_H
