#pragma once
#include <openssl/sha.h>

int random_num_gen();

unsigned char* gen_token(int seed);
