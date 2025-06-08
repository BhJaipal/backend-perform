#include "../lexer/tokens.h"

int main() {
	char jsonStr[] = "{'jaipal': 30, 'data': [true , 43.6,{}]}";
	String str = str_new(jsonStr);
	Tokens tokens = lexer(str);
	token_array_print(tokens, 0);
	token_array_free(tokens);
	return 0;
}
