# Sistema de Visualização Gráfica 3D

Projeto desenvolvido para a disciplina de Processamento Gráfico. Implementa um sistema interativo de visualização 3D com iluminação, câmera FPS e geometrias primitivas usando OpenGL 3.3.

## O que está implementado

- Pipeline gráfico completo (vertex/fragment shaders)
- Transformações 3D com matrizes modelo, visão e projeção
- Iluminação Phong (luz direcional + pontuais com atenuação)
- Câmera FPS com movimento WASD e controle por mouse
- Geometrias procedurais: cubo, esfera e plano
- Sistema de materiais (metálico, plástico, padrão)
- Toggle de wireframe e iluminação em tempo real

## Dependências

- OpenGL 3.3+
- GLFW 3
- GLM
- GLAD (precisa gerar — veja abaixo)

### Instalando no Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libglfw3-dev libglm-dev
```

### macOS

```bash
brew install cmake glfw glm
```

## Compilando

### 1. Gerar o GLAD

Acesse https://glad.dav1d.de/ e configure:
- Language: C/C++
- Specification: OpenGL
- API gl: 3.3 (Core)
- Marque "Generate a loader"

Extraia o zip e copie os arquivos para:
- `glad.c` → `glad/src/`
- `glad.h` e `khrplatform.h` → `glad/include/`

### 2. Compilar

```bash
mkdir build
cd build
cmake ..
make
```

### 3. Executar

```bash
./SistemaVisualizacaoGrafica
```

> Execute sempre de dentro de `build/` após copiar a pasta `shaders/` para lá, ou volte para o diretório raiz antes de rodar.

## Controles

| Tecla | Ação |
|-------|------|
| W / S | Frente / Trás |
| A / D | Esquerda / Direita |
| Espaço / Shift | Subir / Descer |
| Mouse | Rotacionar câmera |
| Scroll | Ajustar FOV |
| L | Liga/desliga iluminação |
| F | Alterna wireframe |
| ESC | Sair |

## Estrutura do projeto

```
├── src/
│   ├── main.cpp       # loop principal e callbacks
│   ├── Shader.h       # carrega e compila shaders GLSL
│   ├── Camera.h       # câmera FPS com ângulos de Euler
│   ├── Mesh.h         # cubo, esfera e plano procedurais
│   └── Light.h        # estruturas de luz e material
├── shaders/
│   ├── vertexShader.glsl
│   ├── fragmentShader.glsl
│   ├── lightingVert.glsl
│   └── lightingFrag.glsl
├── CMakeLists.txt
└── Makefile
```

## Resolução de problemas

**Tela preta:** verifique se os shaders estão sendo encontrados (pasta `shaders/` acessível no diretório de execução) e se GLAD foi inicializado corretamente.

**Objetos não aparecem:** cheque a posição da câmera e as matrizes de transformação. Um objeto pode estar atrás da câmera ou fora do frustum.

**Iluminação estranha:** normais precisam ser renormalizadas no fragment shader após interpolação. Veja `lightingFrag.glsl`.

## Referências

- [Learn OpenGL](https://learnopengl.com/) — Joey de Vries
- *Real-Time Rendering*, 4ª ed. — Akenine-Möller et al.
- [docs.gl](https://docs.gl/) — referência da API OpenGL
