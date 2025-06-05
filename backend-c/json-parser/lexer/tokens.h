#ifndef JSON_TOKENS_H
#define JSON_TOKENS_H

#include "../json-str.h"
#include "../json-arr.h"

typedef enum {
	TT_L_CURLY,
	TT_R_CURLY,
	TT_L_SQ,
	TT_R_SQ,
	TT_STRING,
	TT_INT,
	TT_FLOAT,
	TT_BOOL,
	TT_COMMA,
	TT_COLON,
	TT_INVALID
} TokenType;

typedef struct {
	TokenType type;
	String literal;
} Token;

void token_free(Token token);

Array_t(Tokens, Token, token);

char is_whitespace(char ch);

Tokens lexer(String src);

#endif // !JSON_TOKENS_H
