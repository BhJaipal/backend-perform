#include "dict.h"
#include "pair.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

void dict_print(Dict dict, int depth) {
	printf("{\n");
	Pair *curr = dict->head;
	while (curr != NULL) {
		pair_print(curr, depth + 1);
		curr = curr->next;
	}
	for (int i = 0; i < depth; i++) {
		printf("\t");
	}
	printf("}");
}

Dict dict_new() {
	Dict arr = malloc(sizeof(struct _Dict));
	arr->head = NULL;
	arr->len = 0;
	return arr;
}
void dict_add(Dict arr, Pair *el) {
	arr->len++;
	if (arr->head == NULL) {
		arr->head = el;
		return;
	}
	Pair *curr = arr->head;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = el;
}
void dict_free(Dict array){
	Pair *curr = array->head,
		*prev = NULL;
	if (curr == NULL) return;
	while (curr->next != NULL) {
		prev = curr;
		curr = curr->next;
		pair_free(prev);
	}
	pair_free(curr);
	free(array);
}
