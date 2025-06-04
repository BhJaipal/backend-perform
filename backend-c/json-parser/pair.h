#ifndef JSON_PAIR_H
#define JSON_PAIR_H

#include "value.h"

typedef struct Pr {
	JsonTypes  type;
	String     key;
	Value      value;
	struct Pr *next;
} Pair;

struct _PairBuilder {
	Pair *(*from_i32)(char *key,  int32 val);
	Pair *(*from_f32)(char *key,  float val);
	Pair *(*from_str)(char *key,  String val);
	Pair *(*from_arr)(char *key,  ValueArray arr);
	Pair *(*from_cstr)(char *key, char *cstr);
	Pair *(*from_vals)(char *key, Value *val, unsigned len);
};
extern struct _PairBuilder PairBuilder;

void pair_print(Pair *pair, int level);

void pair_free(Pair *pair);

#endif // !JSON_PAIR_H
