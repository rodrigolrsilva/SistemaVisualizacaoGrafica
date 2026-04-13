#!/bin/bash

# Verifica se todos os arquivos necessários estão presentes

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

MISSING=0
PRESENT=0

check_file() {
    if [ -f "$1" ]; then
        echo -e "${GREEN}ok${NC}  $1"
        ((PRESENT++))
    else
        echo -e "${RED}!!${NC}  $1 ${RED}[FALTANDO]${NC}"
        ((MISSING++))
    fi
}

echo "Código C++"
check_file "src/main.cpp"
check_file "src/Shader.h"
check_file "src/Camera.h"
check_file "src/Mesh.h"
check_file "src/Light.h"

echo ""
echo "Shaders"
check_file "shaders/vertexShader.glsl"
check_file "shaders/fragmentShader.glsl"
check_file "shaders/lightingVert.glsl"
check_file "shaders/lightingFrag.glsl"

echo ""
echo "Build"
check_file "CMakeLists.txt"
check_file "Makefile"

echo ""
echo "GLAD (gerar em https://glad.dav1d.de/ — OpenGL 3.3 Core)"
if [ -f "glad/src/glad.c" ]; then
    echo -e "${GREEN}ok${NC}  glad/src/glad.c"
    ((PRESENT++))
else
    echo -e "${YELLOW}--${NC}  glad/src/glad.c [precisa gerar]"
fi

if [ -f "glad/include/glad/glad.h" ]; then
    echo -e "${GREEN}ok${NC}  glad/include/glad/glad.h"
    ((PRESENT++))
else
    echo -e "${YELLOW}--${NC}  glad/include/glad/glad.h [precisa gerar]"
fi

if [ -f "glad/include/KHR/khrplatform.h" ]; then
    echo -e "${GREEN}ok${NC}  glad/include/KHR/khrplatform.h"
    ((PRESENT++))
else
    echo -e "${YELLOW}--${NC}  glad/include/KHR/khrplatform.h [precisa gerar]"
fi

echo ""
echo "$PRESENT ok, $MISSING faltando"

if [ $MISSING -gt 0 ]; then
    exit 1
fi
