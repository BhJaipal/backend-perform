SRC = ./pair.c ./json-str.c

build:
	gcc $(SRC) -shared -o libjsonc.so
	chmod -x libjsonc.so

test:
	gcc test_1.c ./libjsonc.so
	./a.out
