#include "../pair.h"

int main() {
	Pair *pair = PairBuilder.from_f32("Jaipal", 7.5);
	pair_print(pair, 0);
	pair_free(pair);

	Value vals[2] = {value_new.from_i32(5), value_new.from_str("coding")};
	pair = PairBuilder.from_vals("Hobbies", vals, 2);
	pair_print(pair, 0);
	pair_free(pair);
}
