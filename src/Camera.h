#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Enumeração para movimentos possíveis da câmera
enum MovimentoCamera {
    FRENTE,
    TRAS,
    ESQUERDA,
    DIREITA,
    CIMA,
    BAIXO
};

// Valores padrão da câmera
const float YAW_PADRAO = -90.0f;
const float PITCH_PADRAO = 0.0f;
const float VELOCIDADE_PADRAO = 2.5f;
const float SENSIBILIDADE_PADRAO = 0.1f;
const float ZOOM_PADRAO = 45.0f;

// Classe que implementa uma câmera FPS para navegação 3D
class Camera {
public:
    // Atributos da câmera
    glm::vec3 posicao;
    glm::vec3 direcaoFrente;
    glm::vec3 direcaoCima;
    glm::vec3 direcaoDireita;
    glm::vec3 cimaMundial;

    // Ângulos de Euler
    float yaw;
    float pitch;

    // Opções da câmera
    float velocidadeMovimento;
    float sensibilidadeMouse;
    float zoom;

    // Construtor com vetores
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 cima = glm::vec3(0.0f, 1.0f, 0.0f),
           float angleYaw = YAW_PADRAO,
           float anglePitch = PITCH_PADRAO)
        : direcaoFrente(glm::vec3(0.0f, 0.0f, -1.0f)),
          velocidadeMovimento(VELOCIDADE_PADRAO),
          sensibilidadeMouse(SENSIBILIDADE_PADRAO),
          zoom(ZOOM_PADRAO) {
        
        posicao = pos;
        cimaMundial = cima;
        yaw = angleYaw;
        pitch = anglePitch;
        atualizarVetoresCamera();
    }

    // Construtor com valores escalares
    Camera(float posX, float posY, float posZ,
           float cimaX, float cimaY, float cimaZ,
           float angleYaw, float anglePitch)
        : direcaoFrente(glm::vec3(0.0f, 0.0f, -1.0f)),
          velocidadeMovimento(VELOCIDADE_PADRAO),
          sensibilidadeMouse(SENSIBILIDADE_PADRAO),
          zoom(ZOOM_PADRAO) {
        
        posicao = glm::vec3(posX, posY, posZ);
        cimaMundial = glm::vec3(cimaX, cimaY, cimaZ);
        yaw = angleYaw;
        pitch = anglePitch;
        atualizarVetoresCamera();
    }

    // Retorna a matriz view calculada usando ângulos de Euler e LookAt
    glm::mat4 obterMatrizView() {
        return glm::lookAt(posicao, posicao + direcaoFrente, direcaoCima);
    }

    // Processa entrada do teclado
    void processarTeclado(MovimentoCamera direcao, float deltaTime) {
        float velocidade = velocidadeMovimento * deltaTime;
        
        if (direcao == FRENTE)
            posicao += direcaoFrente * velocidade;
        if (direcao == TRAS)
            posicao -= direcaoFrente * velocidade;
        if (direcao == ESQUERDA)
            posicao -= direcaoDireita * velocidade;
        if (direcao == DIREITA)
            posicao += direcaoDireita * velocidade;
        if (direcao == CIMA)
            posicao += direcaoCima * velocidade;
        if (direcao == BAIXO)
            posicao -= direcaoCima * velocidade;
    }

    // Processa movimento do mouse
    void processarMovimentoMouse(float deslocamentoX, float deslocamentoY, GLboolean limitarPitch = true) {
        deslocamentoX *= sensibilidadeMouse;
        deslocamentoY *= sensibilidadeMouse;

        yaw += deslocamentoX;
        pitch += deslocamentoY;

        // Limitar pitch para evitar flip da câmera
        if (limitarPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        atualizarVetoresCamera();
    }

    // Processa scroll do mouse (zoom)
    void processarScrollMouse(float deslocamentoY) {
        zoom -= deslocamentoY;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    // Calcula o vetor frontal a partir dos ângulos de Euler atualizados
    void atualizarVetoresCamera() {
        glm::vec3 frente;
        frente.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frente.y = sin(glm::radians(pitch));
        frente.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        direcaoFrente = glm::normalize(frente);
        direcaoDireita = glm::normalize(glm::cross(direcaoFrente, cimaMundial));
        direcaoCima = glm::normalize(glm::cross(direcaoDireita, direcaoFrente));
    }
};

#endif
