#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Estrutura para representar um vértice completo
struct Vertice {
    glm::vec3 posicao;
    glm::vec3 normal;
    glm::vec2 coordTextura;
};

// Classe base para malhas geométricas
class Mesh {
public:
    std::vector<Vertice> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;

    // Construtor padrão
    Mesh() : VAO(0), VBO(0), EBO(0) {}

    // Construtor com dados
    Mesh(std::vector<Vertice> verts, std::vector<GLuint> inds) {
        vertices = verts;
        indices = inds;
        configurarMesh();
    }

    // Desenhar a malha
    void desenhar() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Desenhar com modo específico
    void desenhar(GLenum modo) {
        glBindVertexArray(VAO);
        glDrawElements(modo, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // Limpar recursos
    void limpar() {
        if (VAO != 0) {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            VAO = VBO = EBO = 0;
        }
    }

    ~Mesh() {
        limpar();
    }

protected:
    void configurarMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertice), 
                     &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                     &indices[0], GL_STATIC_DRAW);

        // Posição do vértice
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)0);

        // Normal do vértice
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), 
                              (void*)offsetof(Vertice, normal));

        // Coordenadas de textura
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice),
                              (void*)offsetof(Vertice, coordTextura));

        glBindVertexArray(0);
    }
};

// Criar um cubo
class Cubo : public Mesh {
public:
    Cubo(float tamanho = 1.0f) {
        float meio = tamanho / 2.0f;
        
        // Definir vértices do cubo com normais
        vertices = {
            // Face frontal (Z+)
            {{-meio, -meio,  meio}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{ meio, -meio,  meio}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{ meio,  meio,  meio}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-meio,  meio,  meio}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            
            // Face traseira (Z-)
            {{ meio, -meio, -meio}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{-meio, -meio, -meio}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-meio,  meio, -meio}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{ meio,  meio, -meio}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            
            // Face direita (X+)
            {{ meio, -meio,  meio}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{ meio, -meio, -meio}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{ meio,  meio, -meio}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{ meio,  meio,  meio}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            
            // Face esquerda (X-)
            {{-meio, -meio, -meio}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-meio, -meio,  meio}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-meio,  meio,  meio}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-meio,  meio, -meio}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            
            // Face superior (Y+)
            {{-meio,  meio,  meio}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ meio,  meio,  meio}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ meio,  meio, -meio}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-meio,  meio, -meio}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            
            // Face inferior (Y-)
            {{-meio, -meio, -meio}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ meio, -meio, -meio}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{ meio, -meio,  meio}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{-meio, -meio,  meio}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}
        };

        // Definir índices (2 triângulos por face)
        indices = {
            0, 1, 2, 2, 3, 0,       // Frontal
            4, 5, 6, 6, 7, 4,       // Traseira
            8, 9, 10, 10, 11, 8,    // Direita
            12, 13, 14, 14, 15, 12, // Esquerda
            16, 17, 18, 18, 19, 16, // Superior
            20, 21, 22, 22, 23, 20  // Inferior
        };

        configurarMesh();
    }
};

// Criar uma esfera usando coordenadas esféricas
class Esfera : public Mesh {
public:
    Esfera(float raio = 1.0f, int setores = 36, int pilhas = 18) {
        float x, y, z, xy;
        float nx, ny, nz, comprimentoInverso = 1.0f / raio;
        float s, t;

        float setorPasso = 2 * M_PI / setores;
        float pilhaPasso = M_PI / pilhas;
        float anguloSetor, anguloPilha;

        // Gerar vértices
        for (int i = 0; i <= pilhas; i++) {
            anguloPilha = M_PI / 2 - i * pilhaPasso;
            xy = raio * cosf(anguloPilha);
            z = raio * sinf(anguloPilha);

            for (int j = 0; j <= setores; j++) {
                anguloSetor = j * setorPasso;

                x = xy * cosf(anguloSetor);
                y = xy * sinf(anguloSetor);

                nx = x * comprimentoInverso;
                ny = y * comprimentoInverso;
                nz = z * comprimentoInverso;

                s = (float)j / setores;
                t = (float)i / pilhas;

                Vertice vert;
                vert.posicao = glm::vec3(x, y, z);
                vert.normal = glm::vec3(nx, ny, nz);
                vert.coordTextura = glm::vec2(s, t);
                vertices.push_back(vert);
            }
        }

        // Gerar índices
        int k1, k2;
        for (int i = 0; i < pilhas; i++) {
            k1 = i * (setores + 1);
            k2 = k1 + setores + 1;

            for (int j = 0; j < setores; j++, k1++, k2++) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (pilhas - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        configurarMesh();
    }
};

// Criar um plano
class Plano : public Mesh {
public:
    Plano(float largura = 10.0f, float profundidade = 10.0f, int divisoesX = 10, int divisoesZ = 10) {
        float meiaLargura = largura / 2.0f;
        float meiaProfundidade = profundidade / 2.0f;
        
        float passoX = largura / divisoesX;
        float passoZ = profundidade / divisoesZ;
        
        float passoTexX = 1.0f / divisoesX;
        float passoTexZ = 1.0f / divisoesZ;

        // Gerar vértices
        for (int z = 0; z <= divisoesZ; z++) {
            for (int x = 0; x <= divisoesX; x++) {
                Vertice vert;
                vert.posicao = glm::vec3(-meiaLargura + x * passoX, 0.0f, -meiaProfundidade + z * passoZ);
                vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                vert.coordTextura = glm::vec2(x * passoTexX, z * passoTexZ);
                vertices.push_back(vert);
            }
        }

        // Gerar índices
        for (int z = 0; z < divisoesZ; z++) {
            for (int x = 0; x < divisoesX; x++) {
                int topLeft = z * (divisoesX + 1) + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * (divisoesX + 1) + x;
                int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        configurarMesh();
    }
};

#endif
