IMGUI_DIR = imgui
OpenGl = $$(pkg-config --libs --cflags glfw3 glesv2 jsoncpp)
CXXFLAGS = -I$(IMGUI_DIR) -std=c++23 -g -Wall -Wformat -lm $(OpenGl)
Main ?= ./main.cpp

Libs = $(IMGUI_DIR)/imgui.cpp \
		$(IMGUI_DIR)/imgui_draw.cpp \
		$(IMGUI_DIR)/imgui_tables.cpp \
		$(IMGUI_DIR)/imgui_widgets.cpp

Objs := $(foreach lib,$(Libs), $$(basename $(lib) | sed 's/.cpp/.o/g'))

Backend = $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp
BackendOut := $(foreach lib, $(Backend), $$(basename $(lib) | sed 's/.cpp/.o/g'))

compile:
	g++ -c $(Target)

build:
	for i in $(Libs); do make compile Target=$$i; done

backend:
	for i in $(Backend); do \
		g++ $(CXXFLAGS) $$i -c; \
		done

all:
	g++ $(BackendOut) $(Main) $(Objs) $(CXXFLAGS)
