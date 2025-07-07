CC = g++
LIBS = $$(pkg-config --libs --cflags lua)
FLAGS = -rdynamic -fPIC -ldl
MAIN = main.cpp request.cpp server.cpp

all:
	$(CC) $(MAIN) $(LIBS) $(FLAGS) -o server.exe
