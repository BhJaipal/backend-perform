CC = g++
LIBS = $$(pkg-config --libs --cflags lua)
FLAGS = -rdynamic -fPIC -ldl
MAIN ?= main.cpp

all:
	$(CC) $(MAIN) $(LIBS) $(FLAGS) -o server.exe
