#version 330 core

layout (location = 0) in vec3 posicaoAtributo;
layout (location = 1) in vec3 normalAtributo;
layout (location = 2) in vec2 coordTexturaAtributo;

out vec3 posicaoFragmento;
out vec3 normalFragmento;
out vec2 coordTextura;

uniform mat4 modelo;
uniform mat4 visao;
uniform mat4 projecao;

void main() {
    posicaoFragmento = vec3(modelo * vec4(posicaoAtributo, 1.0));

    // transpose(inverse()) necessario para preservar normais com escala nao-uniforme
    normalFragmento = mat3(transpose(inverse(modelo))) * normalAtributo;

    coordTextura = coordTexturaAtributo;

    gl_Position = projecao * visao * vec4(posicaoFragmento, 1.0);
}
