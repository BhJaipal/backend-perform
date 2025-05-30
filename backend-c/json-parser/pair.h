#ifndef JSON_PAIR_H
#define JSON_PAIR_H

#include "json-str.h"
#include "json-arr.h"

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
Array_t(Value, struct Value);

typedef struct Value {
	JsonTypes  type;
	union {
		int32      i32;
		float      f32;
		String     str;
		ValueArray arr;
	} value;
} Value;

typedef struct Pr {
	JsonTypes  type;
	String     key;
	Value      value;
	struct Pr *next;
} Pair;

struct _PairBuilder {
	Pair (*from_i32)(String key,  int32 val);
	Pair (*from_f32)(String key,  float val);
	Pair (*from_str)(String key,  String val);
	Pair (*from_arr)(String key,  ValueArray arr);
	Pair (*from_cstr)(String key, char *cstr);
	Pair (*from_vals)(String key, Value *val, unsigned len);
};
extern struct _PairBuilder PairBuilder;

void print_pair(Pair pair, int level);

void pair_free(Pair *pair);

#endif // !JSON_PAIR_H
