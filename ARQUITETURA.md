# Arquitetura do Sistema

Documentação técnica do Sistema de Visualização Gráfica 3D.

---

## 1. Pipeline Gráfico

### Estágio de Aplicação (CPU)

O loop principal em `main.cpp` segue o padrão clássico:

```cpp
while (!glfwWindowShouldClose(janela)) {
    processarEntrada(janela);           // input
    atualizarLogica(deltaTime);         // lógica da cena
    renderizar();                       // envia para GPU
    glfwSwapBuffers(janela);           // apresenta frame
}
```

### Vertex Shader

Cada vértice passa pela transformação completa Local → Mundo → Câmera → Clip:

```glsl
gl_Position = projecao * visao * modelo * vec4(posicao, 1.0);

// normais precisam de tratamento especial em caso de escala não-uniforme
normalFragmento = mat3(transpose(inverse(modelo))) * normal;
```

A matriz normal (`transpose(inverse(modelo))`) é necessária porque escala não-uniforme distorce vetores direcionais — sem ela as normais ficam erradas nas arestas.

### Rasterização

Feita automaticamente pela GPU: clipping, divisão de perspectiva, conversão de triângulos em fragmentos e interpolação de atributos (normais, UVs, etc.).

### Fragment Shader

Calcula a cor final de cada pixel com o modelo de iluminação Phong.

---

## 2. Sistema de Coordenadas

As transformações acontecem em cascata:

```
Local Space   →[modelo]→   World Space
World Space   →[visão]→    View Space
View Space    →[projeção]→  Clip Space
Clip Space    →[GPU]→       NDC → Screen Space
```

### Matriz Modelo

```cpp
glm::mat4 modelo = glm::mat4(1.0f);
modelo = glm::translate(modelo, posicao);
modelo = glm::rotate(modelo, angulo, eixo);
modelo = glm::scale(modelo, escala);
// ordem: Scale → Rotate → Translate
```

### Matriz Visão (LookAt)

```cpp
glm::mat4 visao = glm::lookAt(
    posicaoCamera,
    posicaoCamera + direcao,
    vetorCima
);
```

LookAt constrói uma base ortonormal: z = direção oposta ao olhar, x = direita, y = cima.

### Matriz Projeção

```cpp
glm::mat4 proj = glm::perspective(
    glm::radians(fov),
    aspectRatio,
    near,
    far
);
```

Mapeia o frustum visível para o cubo NDC [-1, 1]³.

---

## 3. Iluminação Phong

### Componentes

**Ambiente** — iluminação base, simula luz indireta:
```glsl
vec3 ambiente = luz.ambiente * material.ambiente;
```

**Difusa** — proporcional ao ângulo entre a normal e a direção da luz (Lei de Lambert):
```glsl
vec3 direcaoLuz = normalize(luz.posicao - posicaoFragmento);
float diff = max(dot(normal, direcaoLuz), 0.0);
vec3 difusa = luz.difusa * diff * material.difusa;
```

**Especular** — reflexos usando Blinn-Phong (half-vector é mais eficiente que `reflect()`):
```glsl
vec3 halfVector = normalize(direcaoLuz + direcaoVisao);
float spec = pow(max(dot(normal, halfVector), 0.0), brilho);
vec3 especular = luz.especular * spec * material.especular;
```

Cor final: `I = ambiente + difusa + especular`

### Atenuação para Luzes Pontuais

```glsl
float dist = length(luz.posicao - fragmento);
float atenuacao = 1.0 / (Kc + Kl*dist + Kq*dist*dist);
```

Parâmetros usados: Kc = 1.0, Kl = 0.09, Kq = 0.032.

---

## 4. Geometria Procedural

### Estrutura de Vértice

```cpp
struct Vertice {
    glm::vec3 posicao;
    glm::vec3 normal;
    glm::vec2 coordTextura;
};
// 8 floats * 4 bytes = 32 bytes por vértice
```

### Buffer Objects

- **VBO** — dados dos vértices na VRAM
- **EBO** — índices para reuso de vértices
- **VAO** — encapsula a configuração de atributos, facilita troca entre geometrias

```cpp
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, tamanho, dados, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset);
glEnableVertexAttribArray(0);
```

### Geração da Esfera

Usa parametrização esférica (θ = azimutal, φ = polar):

```
x = r * cos(φ) * cos(θ)
y = r * cos(φ) * sin(θ)
z = r * sin(φ)
```

Para esfera centrada na origem, a normal é simplesmente `normalize(posicao)`.

### Topologia

| Geometria | Vértices | Índices |
|-----------|----------|---------|
| Cubo | 24 (4 por face, normais por face) | 36 |
| Esfera | (segs+1)×(pilhas+1) | segs×pilhas×6 |
| Plano | (divX+1)×(divZ+1) | divX×divZ×6 |

---

## 5. Câmera FPS

### Ângulos de Euler

```cpp
direcao.x = cos(yaw) * cos(pitch);
direcao.y = sin(pitch);
direcao.z = sin(yaw) * cos(pitch);
```

Pitch limitado a ±89° para evitar gimbal lock.

### Base Ortonormal

```cpp
frente  = normalize(direcao);
direita = normalize(cross(frente, cimaMundial));
cima    = normalize(cross(direita, frente));
```

Esses três vetores são recalculados a cada frame conforme o mouse se move.

---

## 6. Shaders

### Entradas do Vertex Shader

```glsl
layout (location = 0) in vec3 posicao;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 coordTextura;

uniform mat4 modelo;
uniform mat4 visao;
uniform mat4 projecao;
```

### Saída do Fragment Shader

```glsl
out vec4 corFinal;
```

Normais interpoladas pela GPU precisam ser renormalizadas antes do cálculo de iluminação:
```glsl
vec3 normal = normalize(normalFragmento);
```

---

## 7. Otimizações

**Face culling** — descarta faces traseiras (~50% dos fragmentos):
```cpp
glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);
glFrontFace(GL_CCW);
```

**Depth test** — Z-buffer resolve visibilidade corretamente:
```cpp
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);
```

GPUs modernas fazem Early-Z, descartando fragmentos ocultos antes mesmo do fragment shader.

---

## 8. Debugging

Algumas técnicas úteis durante desenvolvimento:

```glsl
// Visualizar normais
corFinal = vec4(normal * 0.5 + 0.5, 1.0);

// Visualizar profundidade
corFinal = vec4(vec3(gl_FragCoord.z), 1.0);
```

```cpp
// Wireframe
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
```

---

## 9. Extensibilidade

Algumas extensões naturais a partir desta base:

- **Texturas:** adicionar `sampler2D` no fragment shader e coordenadas UV nos vértices
- **Shadow mapping:** renderizar a cena do ponto de vista da luz, gerar depth map, comparar no shader principal
- **Normal mapping:** precisaria da matriz TBN (Tangent-Bitangent-Normal) por vértice
- **Instancing:** `glDrawElementsInstanced()` para múltiplas cópias sem custo de draw call por objeto
