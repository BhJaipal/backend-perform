IMGUI_DIR = imgui
OpenGl = $$(pkg-config --libs --cflags glfw3 glesv2 jsoncpp)
CXXFLAGS = -I$(IMGUI_DIR) -std=c++23 -g -Wall -Wformat -lm $(OpenGl)
Main ?= ./main.cpp

Libs = $(IMGUI_DIR)/imgui.cpp \
		$(IMGUI_DIR)/imgui_draw.cpp \
		$(IMGUI_DIR)/imgui_tables.cpp \
		$(IMGUI_DIR)/imgui_widgets.cpp

Backend = $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp
BackendOut = imgui_impl_glfw.o imgui_impl_opengl3.o

Out := $(foreach lib,$(Libs), $(addsuffix .o, $(basename $(lib))))
Objs := $(foreach lib,$(Out), $$(echo $(lib) | sed 's/imgui\///g'))

compile:
	g++ $(CXXFLAGS) -c $$(echo $(Target) | sed 's/.o/.cpp/g')

build:
	for i in $(Out); do make compile Target=$$i; done

backend:
	for i in $(Backend); do \
		g++ $(CXXFLAGS) $$i -c -I$(IMGUI_DIR); \
		done

all:
	g++ $(BackendOut) $(Main) $(Objs) $(CXXFLAGS) -I$(IMGUI_DIR)
