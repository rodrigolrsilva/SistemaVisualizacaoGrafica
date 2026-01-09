Sistema de Visualização Gráfica 3D Interativo
Visão Geral

Este projeto demonstra domínio dos conceitos fundamentais de computação gráfica, implementando um sistema interativo de visualização 3D com iluminação avançada, câmera FPS e múltiplas geometrias.
Conceitos Implementados
1. Pipeline Gráfico Completo

    Application Stage: Gerenciamento de entrada, física e lógica
    Geometry Processing: Transformações de modelo, visão e projeção
    Rasterization: Conversão de primitivas em fragmentos
    Pixel Processing: Cálculos de iluminação por fragmento

2. Transformações 3D

    Matriz Modelo: Posicionamento e orientação de objetos no mundo
    Matriz Visão: Câmera FPS com movimento livre
    Matriz Projeção: Perspectiva configurável com FOV dinâmico

3. Sistema de Iluminação Phong

    Luz Direcional: Simula fonte de luz distante (sol)
    Luzes Pontuais: Múltiplas fontes com atenuação baseada em distância
    Componentes: Ambiente, difusa e especular
    Blinn-Phong: Reflexos especulares otimizados

4. Geometria Procedural

    Cubo: Primitiva básica com 6 faces
    Esfera: Gerada via coordenadas esféricas (latitude/longitude)
    Plano: Subdividido para demonstrar tesselação

5. Sistema de Câmera

    Movimento WASD (frente, trás, esquerda, direita)
    Espaço/Shift (subir/descer)
    Rotação via mouse
    Zoom via scroll

6. Materiais

    Metálico (alto brilho especular)
    Plástico (baixo brilho especular)
    Padrão (brilho médio)

Estrutura do Projeto

ProjetoGraficos/
├── src/
│   ├── main.cpp           # Programa principal e loop de renderização
│   ├── Shader.h           # Classe para gerenciar shaders GLSL
│   ├── Camera.h           # Sistema de câmera FPS
│   ├── Mesh.h             # Geometria e primitivas
│   └── Light.h            # Estruturas de iluminação e materiais
├── shaders/
│   ├── vertexShader.glsl      # Vertex shader básico
│   ├── fragmentShader.glsl    # Fragment shader básico
│   ├── lightingVert.glsl      # Vertex shader com iluminação
│   └── lightingFrag.glsl      # Fragment shader Phong
├── glad/
│   ├── include/glad/glad.h    # Cabeçalho GLAD
│   └── src/glad.c             # Implementação GLAD
├── CMakeLists.txt         # Configuração CMake
└── README.md              # Esta documentação

Dependências

    OpenGL 3.3+: API gráfica
    GLFW 3: Gerenciamento de janelas e entrada
    GLM: Biblioteca de matemática para gráficos
    GLAD: Carregador de funções OpenGL

Instalação das Dependências
Ubuntu/Debian
bash

sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install libglfw3-dev libglm-dev

macOS (Homebrew)
bash

brew install cmake glfw glm

Windows (MSYS2/MinGW)
bash

pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-glfw
pacman -S mingw-w64-x86_64-glm

Compilação
Gerar GLAD

    Acesse https://glad.dav1d.de/
    Configurações:
        Language: C/C++
        Specification: OpenGL
        API gl: Version 3.3+
        Profile: Core
    Marque "Generate a loader"
    Clique em "GENERATE"
    Baixe o arquivo zip
    Extraia glad.c para glad/src/
    Extraia glad/glad.h e KHR/khrplatform.h para glad/include/

Compilar o Projeto
bash

mkdir build
cd build
cmake ..
make

Executar
bash

./SistemaVisualizacaoGrafica

Controles
Tecla	Ação
W	Mover câmera para frente
S	Mover câmera para trás
A	Mover câmera para esquerda
D	Mover câmera para direita
Espaço	Subir
Shift	Descer
Mouse	Rotacionar visão
Scroll	Zoom (ajustar FOV)
L	Alternar iluminação on/off
F	Alternar modo wireframe
ESC	Sair da aplicação
Arquitetura Técnica
Sistema de Shaders

A classe Shader encapsula:

    Leitura de arquivos GLSL
    Compilação de vertex e fragment shaders
    Linkagem em programa
    Tratamento de erros com logs detalhados
    Funções utilitárias para definir uniforms

Sistema de Câmera

Implementa câmera FPS usando:

    Ângulos de Euler (yaw, pitch)
    Matriz LookAt para transformação view
    Vetores ortonormais (frente, direita, cima)
    Limitação de pitch para evitar gimbal lock

Sistema de Geometria

Classes para criação procedural:

    Vertice: Estrutura com posição, normal e coordenadas de textura
    Mesh: Classe base com VAO/VBO/EBO
    Cubo: 24 vértices (4 por face para normais corretas)
    Esfera: Geração via parametrização esférica
    Plano: Grid subdividido

Sistema de Iluminação

Modelo Phong implementado:

    Luz Ambiente: Iluminação base constante
    Luz Difusa: Baseada no ângulo entre normal e direção da luz
    Luz Especular: Reflexos usando Blinn-Phong (half-vector)
    Atenuação: Para luzes pontuais (constante + linear + quadrática)

Equação de atenuação:

atenuacao = 1.0 / (Kc + Kl * d + Kq * d²)

Onde:

    Kc = constante
    Kl = linear
    Kq = quadrática
    d = distância da luz

Detalhes de Implementação
Pipeline de Renderização

    Limpar buffers: Color e depth
    Configurar shader: Ativar programa
    Definir uniforms: Matrizes, luzes, materiais
    Para cada objeto:
        Calcular matriz modelo
        Enviar matriz para shader
        Configurar material
        Desenhar geometria
    Swap buffers: Apresentar frame

Cálculo de Normais

Para o cubo, cada face tem vértices duplicados para permitir normais corretas por face (flat shading nas arestas).

Para a esfera, normais são calculadas como vetores unitários da origem até cada vértice:
cpp

normal = normalize(posicao - centro)

Transformações

Ordem de multiplicação de matrizes:

gl_Position = projecao * visao * modelo * vec4(posicao, 1.0)

A matriz normal é calculada para evitar distorções:
cpp

matrizNormal = transpose(inverse(matrizModelo))

Extensões Possíveis

    Texturização com imagens
    Shadow mapping para sombras
    Normal mapping para detalhes de superfície
    Skybox para ambiente
    Geometrias adicionais (cilindro, cone, toro)
    Animações por keyframes
    Sistema de partículas
    Post-processing effects
    Frustum culling
    LOD (Level of Detail)

Resolução de Problemas
Tela preta

    Verificar se os shaders compilaram corretamente (checar console)
    Verificar se GLAD foi inicializado
    Verificar se o depth test está habilitado

Objetos não aparecem

    Verificar posição da câmera
    Verificar matrizes de transformação
    Verificar se VAO está sendo bound antes do draw

Iluminação incorreta

    Verificar cálculo de normais
    Verificar se normais estão sendo normalizadas no shader
    Verificar posições das luzes

Performance baixa

    Reduzir subdivisões das geometrias
    Reduzir número de luzes pontuais
    Usar face culling: glEnable(GL_CULL_FACE)

Referências

    Real-Time Rendering, 4th Edition (Akenine-Möller et al.)
    Learn OpenGL (Joey de Vries) - learnopengl.com
    OpenGL Programming Guide (Red Book)
    Computer Graphics: Principles and Practice (Hughes et al.)

Licença

Este projeto é desenvolvido para fins educacionais.
Autor

Desenvolvido como demonstração de conceitos de processamento gráfico.

