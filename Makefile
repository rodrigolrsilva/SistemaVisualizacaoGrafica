# Makefile para Sistema de Visualizacao Grafica 3D

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Isrc -Iglad/include
LIBS     = -lglfw -lGL -ldl -lm

SRCDIR   = src
GLADDIR  = glad/src
BUILDDIR = build
TARGET   = $(BUILDDIR)/SistemaVisualizacaoGrafica

SOURCES = $(SRCDIR)/main.cpp $(GLADDIR)/glad.c
OBJECTS = $(BUILDDIR)/main.o $(BUILDDIR)/glad.o

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR)/glad.o: $(GLADDIR)/glad.c | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)
	@echo "Pronto: $(TARGET)"

run: $(TARGET)
	cd $(BUILDDIR) && ./$(notdir $(TARGET))

clean:
	rm -rf $(BUILDDIR)

rebuild: clean all

install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential libglfw3-dev libglm-dev

.PHONY: all run clean rebuild install-deps
