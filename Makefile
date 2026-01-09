# Makefile para Sistema de Visualização Gráfica 3D

# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Isrc -Iglad/include
LIBS = -lglfw -lGL -ldl -lm

# Diretórios
SRCDIR = src
GLADDIR = glad/src
BUILDDIR = build
TARGET = $(BUILDDIR)/SistemaVisualizacaoGrafica

# Arquivos fonte
SOURCES = $(SRCDIR)/main.cpp $(GLADDIR)/glad.c
OBJECTS = $(BUILDDIR)/main.o $(BUILDDIR)/glad.o

# Regra padrão
all: $(TARGET)

# Criar diretório de build
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compilar main.cpp
$(BUILDDIR)/main.o: $(SRCDIR)/main.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compilar glad.c
$(BUILDDIR)/glad.o: $(GLADDIR)/glad.c | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Linkar executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)
	@echo "Compilação concluída! Execute com: $(TARGET)"

# Executar
run: $(TARGET)
	cd $(BUILDDIR) && ./$(notdir $(TARGET))

# Limpar arquivos gerados
clean:
	rm -rf $(BUILDDIR)

# Recompilar tudo
rebuild: clean all

# Instalar dependências (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential libglfw3-dev libglm-dev

.PHONY: all run clean rebuild install-deps

