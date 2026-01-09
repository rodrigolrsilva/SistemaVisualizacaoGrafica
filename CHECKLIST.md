Checklist de Arquivos do Projeto
Estrutura Completa

Verifique se você tem todos estes arquivos na estrutura correta:

ProjetoGraficos/
├── src/
│   ├── ✅ main.cpp
│   ├── ✅ Shader.h
│   ├── ✅ Camera.h
│   ├── ✅ Mesh.h
│   └── ✅ Light.h
├── shaders/
│   ├── ✅ vertexShader.glsl
│   ├── ✅ fragmentShader.glsl
│   ├── ✅ lightingVert.glsl
│   └── ✅ lightingFrag.glsl
├── glad/
│   ├── include/
│   │   ├── glad/
│   │   │   └── ⚠️ glad.h (você precisa gerar)
│   │   └── KHR/
│   │       └── ⚠️ khrplatform.h (você precisa gerar)
│   └── src/
│       └── ⚠️ glad.c (você precisa gerar)
├── ✅ CMakeLists.txt
├── ✅ Makefile
├── ✅ README.md
├── ✅ INSTALACAO.md
├── ✅ ARQUITETURA.md
├── ✅ EXERCICIOS.md
└── ✅ CHECKLIST.md (este arquivo)

Arquivos Fornecidos (✅)
Código Fonte C++

    src/main.cpp - Programa principal com loop de renderização
    src/Shader.h - Gerenciador de shaders GLSL
    src/Camera.h - Sistema de câmera FPS
    src/Mesh.h - Geometria e primitivas (Cubo, Esfera, Plano)
    src/Light.h - Estruturas de iluminação e materiais

Shaders GLSL

    shaders/vertexShader.glsl - Vertex shader básico
    shaders/fragmentShader.glsl - Fragment shader básico
    shaders/lightingVert.glsl - Vertex shader com iluminação
    shaders/lightingFrag.glsl - Fragment shader Phong

Configuração

    CMakeLists.txt - Build system CMake
    Makefile - Build alternativo

Documentação

    README.md - Documentação principal
    INSTALACAO.md - Guia de instalação passo a passo
    ARQUITETURA.md - Documentação técnica detalhada
    EXERCICIOS.md - Exercícios práticos
    CHECKLIST.md - Este arquivo

Arquivos que Você Precisa Gerar (⚠️)
GLAD (OpenGL Loader)

Você precisa gerar estes arquivos em https://glad.dav1d.de/:

    glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h
    glad/src/glad.c

Como gerar:

    Acesse https://glad.dav1d.de/
    Configure:
        Language: C/C++
        Specification: OpenGL
        API gl: Version 3.3 (ou superior)
        Profile: Core
        Marque: Generate a loader
    Clique em GENERATE
    Baixe o arquivo glad.zip
    Extraia para as pastas corretas conforme a estrutura acima

Verificação Pré-Compilação
Antes de compilar, verifique:

    Todos os arquivos ✅ estão presentes
    Todos os arquivos ⚠️ foram gerados e copiados
    A estrutura de diretórios está correta
    Dependências do sistema instaladas (GLFW, GLM)

Comandos de Verificação
bash

# Verificar estrutura de diretórios
ls -R ProjetoGraficos/

# Verificar arquivos essenciais
ls src/*.cpp src/*.h
ls shaders/*.glsl
ls glad/src/glad.c
ls glad/include/glad/glad.h

# Verificar dependências no Linux
pkg-config --modversion glfw3
ldconfig -p | grep libGL

Passos para Executar
1. Preparação
bash

cd ProjetoGraficos

2. Gerar GLAD (se ainda não fez)

Siga as instruções em INSTALACAO.md seção "Passo 3: Gerar e Instalar GLAD"
3. Compilar (escolha um método)

Método A - CMake:
bash

mkdir build
cd build
cmake ..
make

Método B - Makefile:
bash

make

Método C - Manual:
bash

g++ -std=c++17 -Isrc -Iglad/include \
    src/main.cpp glad/src/glad.c \
    -lglfw -lGL -ldl -lm \
    -o SistemaVisualizacaoGrafica

4. Executar

Com CMake:
bash

cd build
./SistemaVisualizacaoGrafica

Com Makefile:
bash

make run

Manual:
bash

./SistemaVisualizacaoGrafica

Troubleshooting Comum
Erro: "glad.h: No such file or directory"

Solução:

    Verifique se gerou o GLAD corretamente
    Confirme que glad/include/glad/glad.h existe
    Verifique os caminhos de include no comando de compilação

Erro: "GLFW not found"

Solução (Ubuntu/Debian):
bash

sudo apt-get install libglfw3-dev

Solução (macOS):
bash

brew install glfw

Erro: "undefined reference to glfw..."

Solução: Adicione -lglfw ao comando de linkagem
Erro: Tela preta ao executar

Solução:

    Certifique-se de executar do diretório onde a pasta shaders/ está visível
    Ou copie shaders/ para dentro de build/

bash

# Se compilou com CMake
cd build
cp -r ../shaders .
./SistemaVisualizacaoGrafica

Validação Final

Após compilar e executar, você deve ver:

    ✅ Janela 3D com título "Sistema de Visualizacao Grafica 3D"
    ✅ Fundo azul escuro
    ✅ Cubo central girando
    ✅ 4 esferas orbitando
    ✅ Plano servindo de chão
    ✅ 3 pequenos cubos coloridos (indicadores de luz)
    ✅ Iluminação realista com sombras e brilhos
    ✅ Controles de câmera funcionando (WASD + mouse)
    ✅ Mensagem de controles no console

Teste dos Controles

    W/A/S/D - Movimenta a câmera
    Espaço/Shift - Sobe/desce
    Mouse - Rotaciona a visão
    Scroll - Zoom in/out
    L - Liga/desliga iluminação
    F - Alterna modo wireframe
    ESC - Fecha o programa

Próximos Passos

Após validar que tudo funciona:

    Ler README.md para entender o projeto
    Estudar ARQUITETURA.md para conceitos técnicos
    Experimentar modificar o código
    Implementar suas próprias features

Recursos Adicionais
Documentação de Referência

    OpenGL: https://docs.gl/
    GLFW: https://www.glfw.org/docs/latest/
    GLM: https://github.com/g-truc/glm
    GLAD: https://glad.dav1d.de/

Tutoriais

    Learn OpenGL: https://learnopengl.com/
    OpenGL Tutorial: http://www.opengl-tutorial.org/
    Anton's OpenGL: https://antongerdelan.net/opengl/

Comunidades

    r/opengl: Reddit
    OpenGL Forum: https://www.khronos.org/forums/
    Stack Overflow: Tag [opengl]


Conclusão

Este checklist serve como guia para garantir que você tem todos os componentes necessários para compilar e executar o projeto com sucesso.


