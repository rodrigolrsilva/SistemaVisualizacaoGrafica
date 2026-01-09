#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

// Estrutura para representar uma luz pontual
struct LuzPontual {
    glm::vec3 posicao;
    
    glm::vec3 ambiente;
    glm::vec3 difusa;
    glm::vec3 especular;
    
    // Parâmetros de atenuação
    float constante;
    float linear;
    float quadratica;

    LuzPontual() 
        : posicao(0.0f, 0.0f, 0.0f),
          ambiente(0.2f, 0.2f, 0.2f),
          difusa(0.8f, 0.8f, 0.8f),
          especular(1.0f, 1.0f, 1.0f),
          constante(1.0f),
          linear(0.09f),
          quadratica(0.032f) {}

    LuzPontual(glm::vec3 pos, glm::vec3 amb, glm::vec3 dif, glm::vec3 esp)
        : posicao(pos),
          ambiente(amb),
          difusa(dif),
          especular(esp),
          constante(1.0f),
          linear(0.09f),
          quadratica(0.032f) {}
};

// Estrutura para luz direcional (como o sol)
struct LuzDirecional {
    glm::vec3 direcao;
    
    glm::vec3 ambiente;
    glm::vec3 difusa;
    glm::vec3 especular;

    LuzDirecional()
        : direcao(-0.2f, -1.0f, -0.3f),
          ambiente(0.1f, 0.1f, 0.1f),
          difusa(0.5f, 0.5f, 0.5f),
          especular(0.7f, 0.7f, 0.7f) {}

    LuzDirecional(glm::vec3 dir, glm::vec3 amb, glm::vec3 dif, glm::vec3 esp)
        : direcao(dir),
          ambiente(amb),
          difusa(dif),
          especular(esp) {}
};

// Estrutura para luz spot (lanterna)
struct LuzSpot {
    glm::vec3 posicao;
    glm::vec3 direcao;
    float anguloInternoCorte;
    float anguloExternoCorte;
    
    glm::vec3 ambiente;
    glm::vec3 difusa;
    glm::vec3 especular;
    
    float constante;
    float linear;
    float quadratica;

    LuzSpot()
        : posicao(0.0f, 0.0f, 0.0f),
          direcao(0.0f, 0.0f, -1.0f),
          anguloInternoCorte(glm::cos(glm::radians(12.5f))),
          anguloExternoCorte(glm::cos(glm::radians(17.5f))),
          ambiente(0.1f, 0.1f, 0.1f),
          difusa(1.0f, 1.0f, 1.0f),
          especular(1.0f, 1.0f, 1.0f),
          constante(1.0f),
          linear(0.09f),
          quadratica(0.032f) {}
};

// Propriedades de material
struct Material {
    glm::vec3 ambiente;
    glm::vec3 difusa;
    glm::vec3 especular;
    float brilho;

    Material()
        : ambiente(0.2f, 0.2f, 0.2f),
          difusa(0.8f, 0.8f, 0.8f),
          especular(0.5f, 0.5f, 0.5f),
          brilho(32.0f) {}

    Material(glm::vec3 amb, glm::vec3 dif, glm::vec3 esp, float bril)
        : ambiente(amb),
          difusa(dif),
          especular(esp),
          brilho(bril) {}
};

#endif

