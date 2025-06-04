#ifndef JSON_DICT_H
#define JSON_DICT_H

#include "pair.h"
#include "json-arr.h"

typedef struct _Dict {
	Pair	*head;
	unsigned  len;
} *Dict;

void dict_print(Dict arr, int depth);
void dict_add(Dict arr, Pair *el);
void dict_free(Dict array);
Dict dict_new();

void dict_print(Dict dict, int depth);

#endif // !JSON_DICT_H
