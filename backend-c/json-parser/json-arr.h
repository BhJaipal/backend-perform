#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include <stdlib.h>
#include <unistd.h>

#define Array_t(Name, Type, name)\
	typedef struct _##Name{ \
		Type *arr;			\
		unsigned len;		\
	} *Name;				\
	void name##_array_print(Name arr, int depth);	\
	void name##_array_add(Name arr, Type el);		\
	void name##_array_free (Name array);			\
	Name name##_array_new();

#define Array_impl_print(Name, Type, name, print)				\
	void name##_array_print(Name arr, int depth) {				\
		printf("[\n");											\
		for (size_t i = 0; i < arr->len; i++) {					\
			for (int j = 0; j < depth + 1; j++) {				\
				printf("\t");									\
			}													\
			print(arr->arr[i], depth + 1);						\
			if (i != arr->len - 1) { printf(",\n"); }			\
		}														\
		printf("\n");											\
		for (int j = 0; j < depth; j++) {						\
			printf("\t");										\
		}														\
		printf("]");											\
	}

#define Array_impl(Name, Type, name)							\
	void name##_array_add(Name arr, Type el) {					\
		arr->len++; 											\
		arr->arr = realloc(arr->arr, sizeof(Type[arr->len]));	\
	}															\
	void name##_array_free(Name array){							\
		for (unsigned i = 0; i < array->len; i++){				\
			name##_free(array->arr[i]);							\
		}														\
		free(array);											\
	}															\
	Name name##_array_new() {									\
		Name arr = malloc(sizeof(struct _##Name));				\
		arr->arr = malloc(0);									\
		arr->len = 0;											\
		return arr ;											\
	}

#endif // !JSON_ARRAY_H
