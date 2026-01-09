Documentação da Arquitetura
Visão Geral do Sistema

Este documento descreve em detalhes a arquitetura e implementação do Sistema de Visualização Gráfica 3D, explicando as decisões de design e os conceitos de computação gráfica aplicados.
1. Pipeline Gráfico
1.1 Estágio de Aplicação (CPU)

Responsabilidades:

    Processamento de entrada (teclado, mouse)
    Lógica da cena (animações, física)
    Culling e otimizações
    Preparação de dados para GPU

Implementação:
cpp

// Loop principal em main.cpp
while (!glfwWindowShouldClose(janela)) {
    processarEntrada(janela);           // Input
    atualizarLogica(deltaTime);         // Lógica
    renderizar();                       // Enviar para GPU
    glfwSwapBuffers(janela);           // Apresentar
}

1.2 Estágio de Processamento Geométrico (GPU)

Vertex Shader (lightingVert.glsl):

Processa cada vértice individualmente:
glsl

// Transformação completa: Local -> Mundo -> Visão -> Clip
gl_Position = projecao * visao * modelo * vec4(posicao, 1.0);

// Transformar normal para espaço do mundo
normalFragmento = mat3(transpose(inverse(modelo))) * normal;

Matriz Normal: A matriz normal é necessária para transformar normais corretamente quando há escala não-uniforme:

MatrizNormal = transpose(inverse(MatrizModelo))

Por quê?

    Normais são vetores direcionais (w=0)
    Escala não-uniforme distorce normais
    A matriz normal preserva ortogonalidade

1.3 Estágio de Rasterização

Executado automaticamente pela GPU:

    Clipping (frustrums)
    Divisão de perspectiva (w-division)
    Viewport transform
    Scanline conversion (primitiva → fragmentos)
    Interpolação de atributos (normais, cores, UV)

1.4 Estágio de Processamento de Fragmentos

Fragment Shader (lightingFrag.glsl):

Calcula cor final de cada pixel usando modelo de iluminação Phong.
2. Sistema de Coordenadas
2.1 Transformações de Espaço

Local Space (Modelo)
    ↓ [Matriz Modelo]
World Space (Mundo)
    ↓ [Matriz Visão]
View Space (Câmera)
    ↓ [Matriz Projeção]
Clip Space
    ↓ [Perspective Division]
NDC (Normalized Device Coordinates)
    ↓ [Viewport Transform]
Screen Space (Pixels)

2.2 Matriz Modelo

Combina transformações:
cpp

glm::mat4 modelo = glm::mat4(1.0f);
modelo = glm::translate(modelo, posicao);    // Translação
modelo = glm::rotate(modelo, angulo, eixo);  // Rotação
modelo = glm::scale(modelo, escala);         // Escala

Ordem importa! SRT (Scale → Rotate → Translate)
2.3 Matriz Visão

Implementada com LookAt:
cpp

glm::mat4 visao = glm::lookAt(
    posicaoCamera,              // Olho
    posicaoCamera + direcao,    // Alvo
    vetorCima                   // Cima
);

LookAt constrói base ortonormal:

    Direção: z = normalize(olho - alvo)
    Direita: x = normalize(cross(cima, z))
    Cima: y = cross(z, x)

2.4 Matriz Projeção

Perspectiva:
cpp

glm::mat4 proj = glm::perspective(
    glm::radians(fov),    // Campo de visão
    aspectRatio,          // Largura/altura
    near,                 // Plano próximo
    far                   // Plano distante
);

Cria frustum que mapeia espaço 3D visível para cubo NDC [-1,1]³.
3. Sistema de Iluminação
3.1 Modelo de Iluminação Phong

Composto por três componentes:

1. Componente Ambiente (Ia):
glsl

vec3 ambiente = luz.ambiente * material.ambiente;

Iluminação base constante, simula luz indireta.

2. Componente Difusa (Id):
glsl

vec3 direcaoLuz = normalize(luz.posicao - posicaoFragmento);
float diff = max(dot(normal, direcaoLuz), 0.0);
vec3 difusa = luz.difusa * diff * material.difusa;

Lei de Lambert: intensidade proporcional ao cosseno do ângulo entre normal e luz.

3. Componente Especular (Is):
glsl

// Blinn-Phong usando half-vector
vec3 halfVector = normalize(direcaoLuz + direcaoVisao);
float spec = pow(max(dot(normal, halfVector), 0.0), brilho);
vec3 especular = luz.especular * spec * material.especular;

Equação Final:

I = Ia + Id + Is
I = Ka*La + Kd*Ld*(N·L) + Ks*Ls*(H·N)^n

Onde:

    K = coeficiente do material
    L = intensidade da luz
    N = normal
    L = direção da luz
    H = half-vector
    n = brilho (shininess)

3.2 Atenuação para Luzes Pontuais
glsl

float distancia = length(luz.posicao - fragmento);
float atenuacao = 1.0 / (Kc + Kl*d + Kq*d²);

Parâmetros típicos:

    Kc = 1.0 (constante)
    Kl = 0.09 (linear)
    Kq = 0.032 (quadrática)

Simula queda natural da intensidade com distância.
3.3 Blinn-Phong vs Phong Clássico

Phong Clássico:
glsl

vec3 reflexao = reflect(-direcaoLuz, normal);
float spec = pow(max(dot(reflexao, direcaoVisao), 0.0), brilho);

Blinn-Phong (usado no projeto):
glsl

vec3 halfVector = normalize(direcaoLuz + direcaoVisao);
float spec = pow(max(dot(halfVector, normal), 0.0), brilho);

Vantagens Blinn-Phong:

    Mais eficiente (evita reflect)
    Melhor comportamento em ângulos rasantes
    Amplamente usado na indústria

4. Geometria Procedural
4.1 Estrutura de Vértice
cpp

struct Vertice {
    glm::vec3 posicao;       // Coordenadas 3D
    glm::vec3 normal;        // Vetor normal
    glm::vec2 coordTextura;  // Coordenadas UV
};

Total: 8 floats * 4 bytes = 32 bytes por vértice
4.2 Buffer Objects

VBO (Vertex Buffer Object):

    Armazena dados de vértices na GPU
    GL_STATIC_DRAW: dados não mudam
    GL_DYNAMIC_DRAW: dados atualizam frequentemente

EBO (Element Buffer Object):

    Armazena índices para reuso de vértices
    Reduz memória (compartilha vértices)

VAO (Vertex Array Object):

    Encapsula configuração de atributos
    Permite trocar entre geometrias rapidamente

cpp

glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, tamanho, dados, GL_STATIC_DRAW);

// Configurar atributos
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, offset);
glEnableVertexAttribArray(0);

4.3 Geração de Esfera

Usa coordenadas esféricas (θ, φ) → (x, y, z):
cpp

x = raio * cos(φ) * cos(θ)
y = raio * cos(φ) * sin(θ)
z = raio * sin(φ)

Onde:

    θ (theta): ângulo azimutal [0, 2π]
    φ (phi): ângulo polar [-π/2, π/2]

Normais: Para esfera centrada na origem, normal = normalize(posicao)

Coordenadas UV:
cpp

u = θ / (2π)
v = (φ + π/2) / π

4.4 Topologia de Malha

Cubo:

    24 vértices (4 por face)
    36 índices (6 faces * 2 triângulos * 3 vértices)
    Vértices duplicados para normais por face

Esfera:

    (setores + 1) * (pilhas + 1) vértices
    setores * pilhas * 6 índices
    Grid latitude-longitude

Plano:

    (divX + 1) * (divZ + 1) vértices
    divX * divZ * 6 índices
    Grid regular subdividido

5. Sistema de Câmera
5.1 Câmera FPS

Baseada em ângulos de Euler:
cpp

// Yaw: rotação em torno de Y (esquerda/direita)
// Pitch: rotação em torno de X (cima/baixo)

direcao.x = cos(yaw) * cos(pitch)
direcao.y = sin(pitch)
direcao.z = sin(yaw) * cos(pitch)

5.2 Prevenção de Gimbal Lock

Limitamos pitch:
cpp

if (pitch > 89.0f) pitch = 89.0f;
if (pitch < -89.0f) pitch = -89.0f;

Isso evita que a câmera vire de cabeça para baixo.
5.3 Base Ortonormal

Mantemos três vetores perpendiculares:
cpp

frente = normalize(direcao)
direita = normalize(cross(frente, cimaMundial))
cima = normalize(cross(direita, frente))

5.4 Movimento Relativo
cpp

// Frente/trás ao longo da direção da câmera
posicao += frente * velocidade

// Esquerda/direita perpendicular
posicao += direita * velocidade

// Cima/baixo ao longo do eixo Y mundial
posicao += cimaMundial * velocidade

6. Shaders GLSL
6.1 Vertex Shader

Entrada (per-vertex):
glsl

layout (location = 0) in vec3 posicao;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 coordTextura;

Saída (interpolada para fragment shader):
glsl

out vec3 posicaoFragmento;
out vec3 normalFragmento;
out vec2 coordTextura;

Uniforms (constantes):
glsl

uniform mat4 modelo;
uniform mat4 visao;
uniform mat4 projecao;

6.2 Fragment Shader

Entrada (interpolada):
glsl

in vec3 posicaoFragmento;
in vec3 normalFragmento;

Saída:
glsl

out vec4 corFinal;  // RGBA

6.3 Interpolação de Atributos

GPU interpola atributos automaticamente:

Para triângulo com vértices V0, V1, V2 e pesos baricêntricos (α, β, γ):

atributo_fragmento = α*V0 + β*V1 + γ*V2

Normais interpoladas devem ser renormalizadas:
glsl

vec3 normal = normalize(normalFragmento);

7. Otimizações
7.1 Face Culling
cpp

glEnable(GL_CULL_FACE);
glCullFace(GL_BACK);        // Cullar faces traseiras
glFrontFace(GL_CCW);        // Counter-clockwise = frente

Economiza ~50% dos fragmentos processados.
7.2 Depth Testing
cpp

glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);       // Desenha se z < z_buffer

Z-buffer resolve visibilidade, evita sobreposições incorretas.
7.3 Early-Z

GPU moderna testa profundidade antes do fragment shader, descartando fragmentos ocultos.
7.4 Instancing (não implementado, mas útil)

Para múltiplas cópias do mesmo objeto:
cpp

glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, numInstancias);

8. Fluxo de Dados

CPU                                GPU
───────────────────────────────────────────────
Criar geometria
  └─> VBO/EBO/VAO ─────────────> VRAM

Configurar shaders
  └─> Uniforms ─────────────────> Registros

Por frame:
  Update lógica
  Calcular matrizes
  └─> glUniform ────────────────> Shader uniforms
  └─> glDrawElements ───────────> Vertex Shader
                                    ↓
                                  Rasterizer
                                    ↓
                                  Fragment Shader
                                    ↓
                                  Framebuffer
  
glfwSwapBuffers ←────────────────  Apresentar

9. Espaço de Cores e Gamma

Cores são em espaço linear durante cálculos:
glsl

vec3 resultado = ambiente + difusa + especular;

Para display correto, converter para sRGB (gamma 2.2):
glsl

corFinal = vec4(pow(resultado, vec3(1.0/2.2)), 1.0);

(Não implementado no projeto básico)
10. Extensibilidade
10.1 Adicionar Texturas
cpp

// No Mesh.h
GLuint texturaID;

// No shader
uniform sampler2D texturaDifusa;
vec3 corTextura = texture(texturaDifusa, coordTextura).rgb;

10.2 Shadow Mapping

    Renderizar cena do ponto de vista da luz → depth map
    No shader principal, comparar profundidade do fragmento com depth map
    Se maior, está na sombra

10.3 Normal Mapping
glsl

vec3 normalTangente = texture(normalMap, UV).rgb * 2.0 - 1.0;
vec3 normal = TBN * normalTangente;

Requer matriz TBN (Tangent-Bitangent-Normal).
11. Considerações de Performance
11.1 CPU-GPU Sincronização

Evitar:
cpp

glReadPixels()  // Força sync
glFinish()      // Espera GPU terminar

Preferir pipeline assíncrono.
11.2 State Changes

Minimizar mudanças de estado:

    Agrupar objetos por shader
    Agrupar por textura
    Usar UBOs para uniforms compartilhados

11.3 Batch Rendering

Combinar múltiplos objetos em um draw call:
cpp

glDrawElementsInstanced()  // Melhor que loop de glDrawElements()

12. Matemática Vetorial
12.1 Produto Escalar (Dot Product)

A · B = |A| |B| cos(θ)

Usos:

    Calcular ângulo entre vetores
    Projeção
    Teste de orientação

12.2 Produto Vetorial (Cross Product)

A × B = vetor perpendicular a A e B
|A × B| = |A| |B| sin(θ)

Usos:

    Calcular normal de superfície
    Construir base ortonormal
    Determinar "lado" (regra da mão direita)

12.3 Normalização

v_norm = v / |v|

Essencial para:

    Direções (lighting)
    Normais
    Vetores de câmera

13. Debugging
13.1 Visualizar Normais
glsl

corFinal = vec4(normal * 0.5 + 0.5, 1.0);  // Mapear [-1,1] para [0,1]

13.2 Visualizar Profundidade
glsl

float depth = gl_FragCoord.z;
corFinal = vec4(vec3(depth), 1.0);

13.3 Wireframe Mode
cpp

glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

Conclusão

Este sistema demonstra a implementação completa de um pipeline gráfico 3D moderno, integrando:

    Transformações matemáticas
    Geometria procedural
    Iluminação física
    Interatividade em tempo real

A arquitetura modular permite extensão para recursos avançados como texturas, sombras e pós-processamento.

