#include "json-str.h"
#include "pair.h"

int main() {
	Pair pair = PairBuilder.from_f32(str_new("Jaipal"), 7.5);
	print_pair(pair, 0);
	pair_free(&pair);

	Value vals[2] = {{INT, 0}, {STRING, 0}};
	vals[0].value.i32 = 5;
	vals[1].value.str = str_new("coding");
	pair = PairBuilder.from_vals(str_new("Hobbies"), vals, 2);
	print_pair(pair, 1);
	pair_free(&pair);
}
