#pragma once
#include <openssl/sha.h>

#include <string>

extern std::string hex[256];

int random_num_gen();

std::string gen_token(int seed);