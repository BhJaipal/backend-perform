SRC = ./pair.c ./json-str.c ./dict.c ./value.c ./lexer/tokens.c

build:
	gcc $(SRC) -shared -o libjsonc.so -fPIC
	chmod -x libjsonc.so

TEST ?= ./tests/test_1.c
test:
	gcc $(TEST) ./libjsonc.so
	./a.out
