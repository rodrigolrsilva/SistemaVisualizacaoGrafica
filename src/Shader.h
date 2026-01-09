#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Classe para gerenciar programas de shader GLSL
// Responsável por compilação, linkagem e uso de shaders
class Shader {
public:
    GLuint idPrograma;

    // Construtor que carrega e compila shaders a partir de arquivos
    Shader(const char* caminhoVertexShader, const char* caminhoFragmentShader) {
        // Etapa 1: Ler código fonte dos arquivos
        std::string codigoVertex, codigoFragment;
        std::ifstream arquivoVertex, arquivoFragment;

        arquivoVertex.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        arquivoFragment.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            arquivoVertex.open(caminhoVertexShader);
            arquivoFragment.open(caminhoFragmentShader);
            
            std::stringstream streamVertex, streamFragment;
            streamVertex << arquivoVertex.rdbuf();
            streamFragment << arquivoFragment.rdbuf();
            
            arquivoVertex.close();
            arquivoFragment.close();
            
            codigoVertex = streamVertex.str();
            codigoFragment = streamFragment.str();
        }
        catch (std::ifstream::failure& erro) {
            std::cout << "ERRO::SHADER::ARQUIVO_NAO_LIDO: " << erro.what() << std::endl;
        }

        const char* codigoVShader = codigoVertex.c_str();
        const char* codigoFShader = codigoFragment.c_str();

        // Etapa 2: Compilar shaders
        GLuint vertex, fragment;
        
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &codigoVShader, NULL);
        glCompileShader(vertex);
        verificarErrosCompilacao(vertex, "VERTEX");

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &codigoFShader, NULL);
        glCompileShader(fragment);
        verificarErrosCompilacao(fragment, "FRAGMENT");

        // Etapa 3: Criar programa e linkar shaders
        idPrograma = glCreateProgram();
        glAttachShader(idPrograma, vertex);
        glAttachShader(idPrograma, fragment);
        glLinkProgram(idPrograma);
        verificarErrosCompilacao(idPrograma, "PROGRAMA");

        // Limpar shaders após linkagem (já estão no programa)
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Ativar o programa de shader
    void usar() {
        glUseProgram(idPrograma);
    }

    // Funções utilitárias para definir uniforms
    void definirBool(const std::string& nome, bool valor) const {
        glUniform1i(glGetUniformLocation(idPrograma, nome.c_str()), (int)valor);
    }

    void definirInt(const std::string& nome, int valor) const {
        glUniform1i(glGetUniformLocation(idPrograma, nome.c_str()), valor);
    }

    void definirFloat(const std::string& nome, float valor) const {
        glUniform1f(glGetUniformLocation(idPrograma, nome.c_str()), valor);
    }

    void definirVec3(const std::string& nome, const glm::vec3& valor) const {
        glUniform3fv(glGetUniformLocation(idPrograma, nome.c_str()), 1, glm::value_ptr(valor));
    }

    void definirVec3(const std::string& nome, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(idPrograma, nome.c_str()), x, y, z);
    }

    void definirVec4(const std::string& nome, const glm::vec4& valor) const {
        glUniform4fv(glGetUniformLocation(idPrograma, nome.c_str()), 1, glm::value_ptr(valor));
    }

    void definirMat4(const std::string& nome, const glm::mat4& matriz) const {
        glUniformMatrix4fv(glGetUniformLocation(idPrograma, nome.c_str()), 1, GL_FALSE, glm::value_ptr(matriz));
    }

private:
    // Verifica erros de compilação/linkagem
    void verificarErrosCompilacao(GLuint shader, std::string tipo) {
        GLint sucesso;
        GLchar logErro[1024];

        if (tipo != "PROGRAMA") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &sucesso);
            if (!sucesso) {
                glGetShaderInfoLog(shader, 1024, NULL, logErro);
                std::cout << "ERRO::SHADER::COMPILACAO::" << tipo << "\n" << logErro << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &sucesso);
            if (!sucesso) {
                glGetProgramInfoLog(shader, 1024, NULL, logErro);
                std::cout << "ERRO::SHADER::LINKAGEM::" << tipo << "\n" << logErro << std::endl;
            }
        }
    }
};

#endif

