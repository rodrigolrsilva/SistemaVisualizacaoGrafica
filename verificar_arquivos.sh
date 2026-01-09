#!/bin/bash

# Script para verificar se todos os arquivos necessários estão presentes

echo "=========================================="
echo "Verificação de Arquivos do Projeto"
echo "=========================================="
echo ""

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

MISSING_COUNT=0
PRESENT_COUNT=0

# Função para verificar arquivo
check_file() {
    if [ -f "$1" ]; then
        echo -e "${GREEN}✓${NC} $1"
        ((PRESENT_COUNT++))
    else
        echo -e "${RED}✗${NC} $1 ${RED}[FALTANDO]${NC}"
        ((MISSING_COUNT++))
    fi
}

echo ">>> Arquivos de Código C++"
check_file "src/main.cpp"
check_file "src/Shader.h"
check_file "src/Camera.h"
check_file "src/Mesh.h"
check_file "src/Light.h"
echo ""

echo ">>> Shaders GLSL"
check_file "shaders/vertexShader.glsl"
check_file "shaders/fragmentShader.glsl"
check_file "shaders/lightingVert.glsl"
check_file "shaders/lightingFrag.glsl"
echo ""

echo ">>> Configuração de Build"
check_file "CMakeLists.txt"
check_file "Makefile"
echo ""

echo ">>> Documentação"
check_file "README.md"
check_file "INSTALACAO.md"
check_file "ARQUITETURA.md"
check_file "EXERCICIOS.md"
check_file "CHECKLIST.md"
check_file "RESUMO_PROJETO.md"
echo ""

echo ">>> GLAD (você deve gerar estes em https://glad.dav1d.de/)"
if [ -f "glad/src/glad.c" ]; then
    echo -e "${GREEN}✓${NC} glad/src/glad.c"
    ((PRESENT_COUNT++))
else
    echo -e "${YELLOW}⚠${NC} glad/src/glad.c ${YELLOW}[VOCÊ PRECISA GERAR]${NC}"
    echo "   Acesse: https://glad.dav1d.de/"
    echo "   Config: OpenGL 3.3+ Core, Generate a loader"
fi

if [ -f "glad/include/glad/glad.h" ]; then
    echo -e "${GREEN}✓${NC} glad/include/glad/glad.h"
    ((PRESENT_COUNT++))
else
    echo -e "${YELLOW}⚠${NC} glad/include/glad/glad.h ${YELLOW}[VOCÊ PRECISA GERAR]${NC}"
fi

if [ -f "glad/include/KHR/khrplatform.h" ]; then
    echo -e "${GREEN}✓${NC} glad/include/KHR/khrplatform.h"
    ((PRESENT_COUNT++))
else
    echo -e "${YELLOW}⚠${NC} glad/include/KHR/khrplatform.h ${YELLOW}[VOCÊ PRECISA GERAR]${NC}"
fi

echo ""
echo "=========================================="
echo "Resumo:"
echo "  Arquivos presentes: $PRESENT_COUNT"
echo "  Arquivos faltando: $MISSING_COUNT"

if [ $MISSING_COUNT -eq 0 ]; then
    if [ -f "glad/src/glad.c" ] && [ -f "glad/include/glad/glad.h" ]; then
        echo -e "${GREEN}✓ Todos os arquivos estão presentes!${NC}"
        echo ""
        echo "Você pode compilar o projeto agora:"
        echo "  mkdir build && cd build && cmake .. && make"
    else
        echo -e "${YELLOW}⚠ Você ainda precisa gerar os arquivos GLAD${NC}"
        echo "  Veja INSTALACAO.md, Passo 3"
    fi
else
    echo -e "${RED}✗ Alguns arquivos do projeto estão faltando${NC}"
    echo "  Verifique a lista acima"
fi
echo "=========================================="

