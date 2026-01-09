#version 330 core

layout (location = 0) in vec3 posicaoAtributo;

uniform mat4 modelo;
uniform mat4 visao;
uniform mat4 projecao;

void main() {
    gl_Position = projecao * visao * modelo * vec4(posicaoAtributo, 1.0);
}

