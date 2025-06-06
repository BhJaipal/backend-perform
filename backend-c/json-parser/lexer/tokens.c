#include "tokens.h"
#include <stdio.h>
#include <string.h>

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

TokenType token_type_last(Tokens tokens) {
	return (tokens->len != 0) 
		? (tokens->arr[tokens->len - 1].type) 
		: TT_INVALID;
}

char is_valid_number(String literal) {
	char is_fl;
	for (unsigned i = 0; i < literal->len; i++) {
		if (literal->str[i] == '.' && !is_fl) {
			is_fl = 1;
			continue;
		}
		if (literal->str[i] == '.' && is_fl) {
			return 0;
		}
		if (!(literal->str[i] >= '0' && literal->str[i] <= '9')) {
			return 0;
		}
	}
	return is_fl ? 2 : 1;
}

typedef enum {
	process_true,
	process_false,
	process_int,
	process_float,
	process_invalid
} processing_literal_tt;

processing_literal_tt process_literal(String literal) {
	if (literal->len == 4 && strncmp(literal->str, "true", 4)) {
		return process_true;
	}
	if (literal->len == 5 && strncmp(literal->str, "false", 5)) {
		return process_false;
	}
	char valid_num = is_valid_number(literal);
	if (valid_num) {
		return valid_num == 2 ? process_float : process_int;
	}
	return process_invalid;
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
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_STRING, literal});
			// 'strme4874gdhd"' valid
			} else if (!is_dbl_quote && ch == '\'') {
				is_str = 0;
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_STRING, literal});
			} else {
				str_push(literal, ch);
			}
			continue;
		}
		if (ch == '"' || ch == '\'') {
			// ede7eg"" TT_INVALID
			// 3737''   TT_INVALID
			if (literal->len != 0) {
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_INVALID, literal});
				free(literal->str);
				literal->str = NULL;
				literal->len = 0;
				return tokens;
			}
			TokenType last_tt = token_type_last(tokens);
			// ,"" not (ex: 3734, "etehd")
			// :'' not (ex: "635rg": "73gd84")
			// [ "yeyd"
			// { "2736eshh"
			// TT_INVALID
			if (tokens->len != 0
				&& last_tt != TT_COMMA
				&& last_tt != TT_COLON
				&& last_tt != TT_L_CURLY
				&& last_tt != TT_L_SQ
			) {
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_INVALID, literal});
				free(literal->str);
				literal->str = NULL;
				literal->len = 0;
				return tokens;
			}
			// Start string
			if (ch == '"') is_dbl_quote = 1;
			is_str = 1;
			str_push(literal, ch);
			continue;
		}
		// "tet": Only valid
		if (ch == ':') {
			if (literal->len != 0) {
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_INVALID, literal});
				free(literal->str);
				literal->str = NULL;
				literal->len = 0;
				return tokens;
			}
			TokenType last_tt = token_type_last(tokens);
			// "tet": Only valid
			if (tokens->len != 0 && last_tt != TT_STRING) {
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_INVALID, literal});
				free(literal->str);
				literal->str = NULL;
				literal->len = 0;
				return tokens;
			}
			str_push(literal, ch);
			token_array_add(tokens, (Token){TT_INVALID, literal});
			free(literal->str);
			literal->str = NULL;
			literal->len = 0;
			continue;
		}
		// "g36d"|38.47|7364|true|false ] or } or ,
		if (ch == '}' || ch == ']' || ch == ',') {
			if (literal->len != 0) {
				processing_literal_tt tt = process_literal(literal);
				switch (tt) {
					case process_true:
						token_array_add(tokens, (Token){TT_BOOL, literal});
					case process_false:
						token_array_add(tokens, (Token){TT_BOOL, literal});
					case process_float:
						token_array_add(tokens, (Token){TT_FLOAT, literal});
					case process_int:
						token_array_add(tokens, (Token){TT_INT, literal});
					default:
						break;
				}
			}
			TokenType last_tt = token_type_last(tokens);
			if (tokens->len != 0
				// 5.6,
				&& last_tt != TT_FLOAT
				// 5,
				&& last_tt != TT_INT
				// "e",
				&& last_tt != TT_STRING
				// true,
				&& last_tt != TT_BOOL
				// ,,
				&& last_tt != TT_COMMA
				// [, or {,
				&& !(ch == ',' && (last_tt == TT_L_SQ || last_tt == TT_L_CURLY))
				// []
				&& !(ch == ']' && last_tt == TT_L_SQ)
				// {}
				&& !(ch == '}' && last_tt == TT_L_CURLY)
			) {
				str_push(literal, ch);
				token_array_add(tokens, (Token){TT_INVALID, literal});
				free(literal->str);
				literal->str = NULL;
				literal->len = 0;
				return tokens;
			}
			TokenType tt;
			switch (ch) {
				case '[':
					tt = TT_L_SQ;
					break;
				case '{':
					tt = TT_L_CURLY;
					break;
				default:
					tt = TT_COMMA;
					break;
			}
			token_array_add(tokens, (Token){tt, literal});
			continue;
		}
		// only numbers and booleans support remaining
		// and [ and { also
		char is_number = is_valid_number(literal);
	}
	return tokens;
}
