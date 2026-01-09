/* Sistema de Visualização Gráfica 3D Interativo
 * Demonstração de conceitos fundamentais de computação gráfica
 * 
 * Características:
 * - Pipeline gráfico completo
 * - Transformações 3D (modelo, visão, projeção)
 * - Sistema de iluminação Phong
 * - Câmera interativa FPS
 * - Múltiplas geometrias primitivas
 * - Texturização e materiais
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

// Callbacks de entrada
void callbackRedimensionamento(GLFWwindow* janela, int largura, int altura);
void callbackMouse(GLFWwindow* janela, double posX, double posY);
void callbackScroll(GLFWwindow* janela, double deslocX, double deslocY);
void processarEntrada(GLFWwindow* janela);

// Configurações da janela
const unsigned int LARGURA_JANELA = 1280;
const unsigned int ALTURA_JANELA = 720;

// Câmera
Camera camera(glm::vec3(0.0f, 2.0f, 8.0f));
float ultimoPosX = LARGURA_JANELA / 2.0f;
float ultimoPosY = ALTURA_JANELA / 2.0f;
bool primeiroMouse = true;

// Temporização
float tempoAnterior = 0.0f;
float deltaTime = 0.0f;

// Estados da aplicação
bool modoWireframe = false;
bool iluminacaoAtivada = true;
float rotacaoObjetos = 0.0f;

int main() {
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Criar janela
    GLFWwindow* janela = glfwCreateWindow(LARGURA_JANELA, ALTURA_JANELA, 
                                          "Sistema de Visualizacao Grafica 3D", NULL, NULL);
    if (janela == NULL) {
        std::cout << "ERRO: Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(janela);
    glfwSetFramebufferSizeCallback(janela, callbackRedimensionamento);
    glfwSetCursorPosCallback(janela, callbackMouse);
    glfwSetScrollCallback(janela, callbackScroll);

    // Capturar mouse
    glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Carregar funções OpenGL via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERRO: Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Configurar OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // Compilar shaders
    Shader shaderIluminacao("shaders/lightingVert.glsl", "shaders/lightingFrag.glsl");
    Shader shaderLuz("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    // Criar geometrias
    Cubo cubo(1.0f);
    Esfera esfera(0.8f, 36, 18);
    Plano plano(20.0f, 20.0f, 20, 20);

    // Configurar iluminação
    LuzDirecional luzDirecional(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        glm::vec3(0.1f, 0.1f, 0.15f),
        glm::vec3(0.4f, 0.4f, 0.5f),
        glm::vec3(0.6f, 0.6f, 0.7f)
    );

    std::vector<LuzPontual> luzesPontuais;
    
    // Luz pontual 1 - vermelha
    luzesPontuais.push_back(LuzPontual(
        glm::vec3(3.0f, 2.0f, 3.0f),
        glm::vec3(0.15f, 0.05f, 0.05f),
        glm::vec3(0.8f, 0.2f, 0.2f),
        glm::vec3(1.0f, 0.3f, 0.3f)
    ));

    // Luz pontual 2 - azul
    luzesPontuais.push_back(LuzPontual(
        glm::vec3(-3.0f, 2.0f, 3.0f),
        glm::vec3(0.05f, 0.05f, 0.15f),
        glm::vec3(0.2f, 0.2f, 0.8f),
        glm::vec3(0.3f, 0.3f, 1.0f)
    ));

    // Luz pontual 3 - verde
    luzesPontuais.push_back(LuzPontual(
        glm::vec3(0.0f, 3.0f, -3.0f),
        glm::vec3(0.05f, 0.15f, 0.05f),
        glm::vec3(0.2f, 0.8f, 0.2f),
        glm::vec3(0.3f, 1.0f, 0.3f)
    ));

    // Materiais
    Material materialPadrao(
        glm::vec3(0.2f, 0.2f, 0.25f),
        glm::vec3(0.7f, 0.7f, 0.8f),
        glm::vec3(0.8f, 0.8f, 0.9f),
        64.0f
    );

    Material materialMetalico(
        glm::vec3(0.25f, 0.25f, 0.25f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.95f, 0.95f, 0.95f),
        128.0f
    );

    Material materialPlastico(
        glm::vec3(0.2f, 0.15f, 0.1f),
        glm::vec3(0.8f, 0.6f, 0.4f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        16.0f
    );

    std::cout << "\n=== CONTROLES ===" << std::endl;
    std::cout << "WASD: Mover camera" << std::endl;
    std::cout << "Espaco/Shift: Subir/Descer" << std::endl;
    std::cout << "Mouse: Rotacionar visao" << std::endl;
    std::cout << "Scroll: Zoom" << std::endl;
    std::cout << "L: Alternar iluminacao" << std::endl;
    std::cout << "F: Alternar wireframe" << std::endl;
    std::cout << "ESC: Sair\n" << std::endl;

    // Loop de renderização
    while (!glfwWindowShouldClose(janela)) {
        // Calcular deltaTime
        float tempoAtual = glfwGetTime();
        deltaTime = tempoAtual - tempoAnterior;
        tempoAnterior = tempoAtual;

        // Processar entrada
        processarEntrada(janela);

        // Renderização
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Atualizar rotação dos objetos
        rotacaoObjetos += 20.0f * deltaTime;

        // Configurar shader de iluminação
        shaderIluminacao.usar();

        // Configurar matrizes de transformação
        glm::mat4 projecao = glm::perspective(glm::radians(camera.zoom),
            (float)LARGURA_JANELA / (float)ALTURA_JANELA, 0.1f, 100.0f);
        glm::mat4 visao = camera.obterMatrizView();

        shaderIluminacao.definirMat4("projecao", projecao);
        shaderIluminacao.definirMat4("visao", visao);
        shaderIluminacao.definirVec3("posicaoObservador", camera.posicao);

        // Configurar iluminação no shader
        if (iluminacaoAtivada) {
            // Luz direcional
            shaderIluminacao.definirVec3("luzDirecional.direcao", luzDirecional.direcao);
            shaderIluminacao.definirVec3("luzDirecional.ambiente", luzDirecional.ambiente);
            shaderIluminacao.definirVec3("luzDirecional.difusa", luzDirecional.difusa);
            shaderIluminacao.definirVec3("luzDirecional.especular", luzDirecional.especular);

            // Luzes pontuais
            for (size_t i = 0; i < luzesPontuais.size(); i++) {
                std::string base = "luzesPontuais[" + std::to_string(i) + "]";
                shaderIluminacao.definirVec3(base + ".posicao", luzesPontuais[i].posicao);
                shaderIluminacao.definirVec3(base + ".ambiente", luzesPontuais[i].ambiente);
                shaderIluminacao.definirVec3(base + ".difusa", luzesPontuais[i].difusa);
                shaderIluminacao.definirVec3(base + ".especular", luzesPontuais[i].especular);
                shaderIluminacao.definirFloat(base + ".constante", luzesPontuais[i].constante);
                shaderIluminacao.definirFloat(base + ".linear", luzesPontuais[i].linear);
                shaderIluminacao.definirFloat(base + ".quadratica", luzesPontuais[i].quadratica);
            }
            shaderIluminacao.definirInt("numLuzesPontuais", luzesPontuais.size());
        } else {
            shaderIluminacao.definirInt("numLuzesPontuais", 0);
            shaderIluminacao.definirVec3("luzDirecional.ambiente", glm::vec3(0.3f));
            shaderIluminacao.definirVec3("luzDirecional.difusa", glm::vec3(0.0f));
            shaderIluminacao.definirVec3("luzDirecional.especular", glm::vec3(0.0f));
        }

        // Desenhar plano (chão)
        glm::mat4 modelo = glm::mat4(1.0f);
        modelo = glm::translate(modelo, glm::vec3(0.0f, -1.0f, 0.0f));
        shaderIluminacao.definirMat4("modelo", modelo);
        shaderIluminacao.definirVec3("material.ambiente", materialPlastico.ambiente);
        shaderIluminacao.definirVec3("material.difusa", materialPlastico.difusa);
        shaderIluminacao.definirVec3("material.especular", materialPlastico.especular);
        shaderIluminacao.definirFloat("material.brilho", materialPlastico.brilho);
        plano.desenhar();

        // Desenhar cubo central
        modelo = glm::mat4(1.0f);
        modelo = glm::translate(modelo, glm::vec3(0.0f, 1.0f, 0.0f));
        modelo = glm::rotate(modelo, glm::radians(rotacaoObjetos), glm::vec3(0.0f, 1.0f, 0.0f));
        modelo = glm::rotate(modelo, glm::radians(rotacaoObjetos * 0.5f), glm::vec3(1.0f, 0.0f, 0.0f));
        shaderIluminacao.definirMat4("modelo", modelo);
        shaderIluminacao.definirVec3("material.ambiente", materialMetalico.ambiente);
        shaderIluminacao.definirVec3("material.difusa", materialMetalico.difusa);
        shaderIluminacao.definirVec3("material.especular", materialMetalico.especular);
        shaderIluminacao.definirFloat("material.brilho", materialMetalico.brilho);
        cubo.desenhar();

        // Desenhar esferas ao redor
        for (int i = 0; i < 4; i++) {
            float angulo = i * 90.0f + rotacaoObjetos * 0.3f;
            float raio = 3.5f;
            float x = raio * cos(glm::radians(angulo));
            float z = raio * sin(glm::radians(angulo));
            float y = 0.5f + 0.3f * sin(glm::radians(rotacaoObjetos * 2.0f + i * 45.0f));

            modelo = glm::mat4(1.0f);
            modelo = glm::translate(modelo, glm::vec3(x, y, z));
            modelo = glm::rotate(modelo, glm::radians(rotacaoObjetos), glm::vec3(1.0f, 1.0f, 0.0f));
            shaderIluminacao.definirMat4("modelo", modelo);
            shaderIluminacao.definirVec3("material.ambiente", materialPadrao.ambiente);
            shaderIluminacao.definirVec3("material.difusa", materialPadrao.difusa);
            shaderIluminacao.definirVec3("material.especular", materialPadrao.especular);
            shaderIluminacao.definirFloat("material.brilho", materialPadrao.brilho);
            esfera.desenhar();
        }

        // Desenhar indicadores visuais das luzes pontuais
        shaderLuz.usar();
        shaderLuz.definirMat4("projecao", projecao);
        shaderLuz.definirMat4("visao", visao);

        for (size_t i = 0; i < luzesPontuais.size(); i++) {
            modelo = glm::mat4(1.0f);
            modelo = glm::translate(modelo, luzesPontuais[i].posicao);
            modelo = glm::scale(modelo, glm::vec3(0.15f));
            shaderLuz.definirMat4("modelo", modelo);
            shaderLuz.definirVec4("cor", glm::vec4(luzesPontuais[i].difusa, 1.0f));
            cubo.desenhar();
        }

        // Trocar buffers e processar eventos
        glfwSwapBuffers(janela);
        glfwPollEvents();
    }

    // Limpeza
    glfwTerminate();
    return 0;
}

void processarEntrada(GLFWwindow* janela) {
    if (glfwGetKey(janela, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(janela, true);

    // Movimento da câmera
    if (glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS)
        camera.processarTeclado(FRENTE, deltaTime);
    if (glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS)
        camera.processarTeclado(TRAS, deltaTime);
    if (glfwGetKey(janela, GLFW_KEY_A) == GLFW_PRESS)
        camera.processarTeclado(ESQUERDA, deltaTime);
    if (glfwGetKey(janela, GLFW_KEY_D) == GLFW_PRESS)
        camera.processarTeclado(DIREITA, deltaTime);
    if (glfwGetKey(janela, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processarTeclado(CIMA, deltaTime);
    if (glfwGetKey(janela, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processarTeclado(BAIXO, deltaTime);

    // Alternar wireframe
    static bool teclaFPressionadaAntes = false;
    if (glfwGetKey(janela, GLFW_KEY_F) == GLFW_PRESS && !teclaFPressionadaAntes) {
        modoWireframe = !modoWireframe;
        glPolygonMode(GL_FRONT_AND_BACK, modoWireframe ? GL_LINE : GL_FILL);
        teclaFPressionadaAntes = true;
    }
    if (glfwGetKey(janela, GLFW_KEY_F) == GLFW_RELEASE) {
        teclaFPressionadaAntes = false;
    }

    // Alternar iluminação
    static bool teclaLPressionadaAntes = false;
    if (glfwGetKey(janela, GLFW_KEY_L) == GLFW_PRESS && !teclaLPressionadaAntes) {
        iluminacaoAtivada = !iluminacaoAtivada;
        teclaLPressionadaAntes = true;
        std::cout << "Iluminacao: " << (iluminacaoAtivada ? "ATIVADA" : "DESATIVADA") << std::endl;
    }
    if (glfwGetKey(janela, GLFW_KEY_L) == GLFW_RELEASE) {
        teclaLPressionadaAntes = false;
    }
}

void callbackRedimensionamento(GLFWwindow* janela, int largura, int altura) {
    glViewport(0, 0, largura, altura);
}

void callbackMouse(GLFWwindow* janela, double posX, double posY) {
    if (primeiroMouse) {
        ultimoPosX = posX;
        ultimoPosY = posY;
        primeiroMouse = false;
    }

    float deslocX = posX - ultimoPosX;
    float deslocY = ultimoPosY - posY;

    ultimoPosX = posX;
    ultimoPosY = posY;

    camera.processarMovimentoMouse(deslocX, deslocY);
}

void callbackScroll(GLFWwindow* janela, double deslocX, double deslocY) {
    camera.processarScrollMouse(deslocY);
}

