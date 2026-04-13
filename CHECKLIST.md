# Checklist do Projeto

Antes de compilar, confirme que sua estrutura de diretórios está assim:

```
SistemaVisualizacaoGrafica/
├── src/
│   ├── main.cpp
│   ├── Shader.h
│   ├── Camera.h
│   ├── Mesh.h
│   └── Light.h
├── shaders/
│   ├── vertexShader.glsl
│   ├── fragmentShader.glsl
│   ├── lightingVert.glsl
│   └── lightingFrag.glsl
├── glad/
│   ├── include/glad/glad.h       ← você precisa gerar
│   ├── include/KHR/khrplatform.h ← você precisa gerar
│   └── src/glad.c                ← você precisa gerar
├── CMakeLists.txt
└── Makefile
```

## Gerando o GLAD

Os arquivos do GLAD não estão incluídos no repositório porque dependem da sua plataforma. Gere em https://glad.dav1d.de/ com as seguintes configurações:

- Language: **C/C++**
- Specification: **OpenGL**
- API gl: **3.3** (ou superior)
- Profile: **Core**
- Marque: **Generate a loader**

Baixe o zip, extraia e copie os arquivos para as pastas indicadas acima.

## Compilando

**CMake (recomendado):**
```bash
mkdir build
cd build
cmake ..
make
```

**Makefile direto:**
```bash
make
```

**Na mão (Linux):**
```bash
g++ -std=c++17 -Isrc -Iglad/include \
    src/main.cpp glad/src/glad.c \
    -lglfw -lGL -ldl -lm \
    -o SistemaVisualizacaoGrafica
```

## Executando

```bash
# CMake
cd build
./SistemaVisualizacaoGrafica

# Makefile
make run
```

> **Atenção:** execute sempre a partir do diretório raiz do projeto (ou copie a pasta `shaders/` para dentro de `build/`), senão os shaders não são encontrados.

## O que você deve ver

Ao rodar, deve aparecer uma janela 3D com:
- Fundo azul escuro
- Um cubo central girando
- 4 esferas orbitando ao redor
- Um plano como chão
- 3 cubinhos coloridos indicando as luzes
- Iluminação com sombras e brilhos (Phong)

## Controles

| Tecla | Ação |
|-------|------|
| W/A/S/D | Mover câmera |
| Espaço / Shift | Subir / Descer |
| Mouse | Rotacionar visão |
| Scroll | Zoom |
| L | Liga/desliga iluminação |
| F | Alterna wireframe |
| ESC | Fechar |

## Erros comuns

**`glad.h: No such file or directory`** — GLAD não foi gerado ou está no lugar errado. Confirme que `glad/include/glad/glad.h` existe.

**`GLFW not found`** — Instale a dependência:
```bash
# Ubuntu/Debian
sudo apt-get install libglfw3-dev

# macOS
brew install glfw
```

**Tela preta** — Provavelmente os shaders não estão sendo encontrados. Veja a nota sobre o diretório de execução acima.
