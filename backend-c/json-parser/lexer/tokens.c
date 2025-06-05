#include "tokens.h"
#include <stdio.h>

void token_free(Token token) {
	str_free(token.literal);
}

void token_print(Token token, int depth) {
	printf("{ ");
	switch (token.type) {
		case TT_L_CURLY:
			printf("Left Curly, {");
			break;
		case TT_R_CURLY:
			printf("Right Curly, }");
			break;
		case TT_L_SQ:
			printf("Left Square, [");
			break;
		case TT_R_SQ:
			printf("Right Square, ]");
			break;
		case TT_STRING:
			printf("String, %s", token.literal->str);
			break;
		case TT_INT:
			printf("Int, %s", token.literal->str);
			break;
		case TT_FLOAT:
			printf("Float, %s", token.literal->str);
			break;
		case TT_BOOL:
			printf("Bool, %s", token.literal->str);
			break;
		case TT_COLON:
			printf("Colon, ':'");
			break;
		case TT_COMMA:
			printf("Comma, ','");
			break;
		default:
			printf("Illegal, %s", token.literal->str);
			break;
	}
	printf(" }");
}

Array_impl(Tokens, Token, token);

char is_whitespace(char ch) {
	char whitespaces[] = " \r\b\t\n";
	for (int i = 0; i < 5; i++) {
		if (ch == whitespaces[i]) {
			return 1;
		}
	}
	return 0;
}

Tokens lexer(String src) {
	Tokens tokens = token_array_new();
	String literal;
	char prev_ch = 0, is_comment = 0, 
		is_str = 0, is_dbl_quote = 0,
		is_int = 0, is_num = 0,
		is_bool = 0;
	for (unsigned i = 0; i < src->len; i++) {
		char ch = src->str[i];
		// comments supported json
		// if current line is comment and reached line end, stop comments
		if (ch == '\n' && is_comment) {
			is_comment = 0;
			continue;
		}
		// if //, start comment but it shouldn't be in string
		else if (prev_ch == '/' && ch == '/' && !is_str) {
			is_comment = 1;
			continue;
		}
		else if (is_str) {
			// "strme4874gd'" valid
			if (is_dbl_quote && ch == '"') {
				is_str = 0;
				is_dbl_quote = 0;
				token_array_add(tokens, (Token){TT_STRING, literal});
			// 'strme4874gdhd"' valid
			} else if (!is_dbl_quote && ch == '\'') {
				is_str = 0;
				token_array_add(tokens, (Token){TT_STRING, literal});
			} else {
				str_push(literal, ch);
			}
			continue;
		}
	}
	return tokens;
}
