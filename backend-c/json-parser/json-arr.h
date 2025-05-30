#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include <unistd.h>

#define Array_t(Name, Type) \
	typedef struct _##Name##Arr {\
		Type *arr;        \
		unsigned len;     \
	} *Name##Array;       \
	void Name##_array_print(Name##Array arr, int depth);\
	void Name##_array_free(Name##Array array);

#define Array_impl(Name, Type, name)                          \
	void Name##_array_print(Name##Array arr, int depth) {     \
		printf("[\n");                                        \
		for (size_t i = 0; i < arr->len; i++) {               \
			for (int j = 0; j < depth + 1; j++) {             \
				printf("\t");                                 \
			}                                                 \
			name##_print(arr->arr[i], depth + 1);             \
			if (i != arr->len - 1) { printf(",\n"); }         \
		}                                                     \
		printf("\n");                                         \
		for (int j = 0; j < depth; j++) {                     \
			printf("\t");                                     \
		}                                                     \
		printf("]");                                          \
	}                                                         \
	void Name##_array_free(Name##Array array) {               \
		for (unsigned i = 0; i < array->len; i++) {           \
			name##_free(array->arr[i]);                       \
		}                                                     \
		free(array);                                          \
	}

#endif // !JSON_ARRAY_H
