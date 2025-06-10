#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib.h"

int random_num_gen() {
	srand(time(0));
	return (rand() + 1) % 256;
}

unsigned char* gen_token(int seed) {
	char num[6];
	snprintf(num, 6, "%d", seed);
	unsigned char *num_hash = malloc(sizeof(char[32]));
	SHA256((const unsigned char*)num, 6, num_hash);
	return num_hash;
}
