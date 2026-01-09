cmake_minimum_required(VERSION 3.10)
project(SistemaVisualizacaoGrafica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar pacotes necessários
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Incluir diretórios
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/glad/include
)

# Arquivos fonte
set(SOURCES
    src/main.cpp
    glad/src/glad.c
)

# Criar executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    ${CMAKE_DL_LIBS}
)

# Copiar pasta de shaders para o diretório de build
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

