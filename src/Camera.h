#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MovimentoCamera {
    FRENTE,
    TRAS,
    ESQUERDA,
    DIREITA,
    CIMA,
    BAIXO
};

const float YAW_PADRAO          = -90.0f;
const float PITCH_PADRAO        =   0.0f;
const float VELOCIDADE_PADRAO   =   2.5f;
const float SENSIBILIDADE_PADRAO =  0.1f;
const float ZOOM_PADRAO         =  45.0f;

class Camera {
public:
    glm::vec3 posicao;
    glm::vec3 direcaoFrente;
    glm::vec3 direcaoCima;
    glm::vec3 direcaoDireita;
    glm::vec3 cimaMundial;

    // ângulos de Euler
    float yaw;
    float pitch;

    float velocidadeMovimento;
    float sensibilidadeMouse;
    float zoom;

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

    glm::mat4 obterMatrizView() {
        return glm::lookAt(posicao, posicao + direcaoFrente, direcaoCima);
    }

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

    void processarMovimentoMouse(float deslocamentoX, float deslocamentoY, GLboolean limitarPitch = true) {
        deslocamentoX *= sensibilidadeMouse;
        deslocamentoY *= sensibilidadeMouse;

        yaw   += deslocamentoX;
        pitch += deslocamentoY;

        // limitar pitch para evitar flip da câmera
        if (limitarPitch) {
            if (pitch >  89.0f) pitch =  89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        atualizarVetoresCamera();
    }

    void processarScrollMouse(float deslocamentoY) {
        zoom -= deslocamentoY;
        if (zoom <  1.0f) zoom =  1.0f;
        if (zoom > 45.0f) zoom = 45.0f;
    }

private:
    void atualizarVetoresCamera() {
        glm::vec3 frente;
        frente.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frente.y = sin(glm::radians(pitch));
        frente.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        direcaoFrente = glm::normalize(frente);
        direcaoDireita = glm::normalize(glm::cross(direcaoFrente, cimaMundial));
        direcaoCima    = glm::normalize(glm::cross(direcaoDireita, direcaoFrente));
    }
};

#endif
