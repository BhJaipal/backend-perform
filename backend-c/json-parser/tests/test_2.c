#include <stdio.h>
#include "../dict.h"

int main() {
	Pair *pair = PairBuilder.from_f32("Jaipal", 7.5);

	Value vals[2] = {value_new.from_i32(5), value_new.from_str("coding")};
	Pair *pair2 = PairBuilder.from_vals("Hobbies", vals, 2);

	Dict dict = dict_new();
	dict_add(dict, pair);
	dict_add(dict, pair2);

	dict_print(dict, 0);
	printf("\n");
	dict_free(dict);
}
