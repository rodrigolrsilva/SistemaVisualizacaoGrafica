#version 330 core

out vec4 corFinal;

in vec3 posicaoFragmento;
in vec3 normalFragmento;
in vec2 coordTextura;

// Estrutura para luz direcional
struct LuzDirecional {
    vec3 direcao;
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
};

// Estrutura para luz pontual
struct LuzPontual {
    vec3 posicao;
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
    float constante;
    float linear;
    float quadratica;
};

// Estrutura para material
struct Material {
    vec3 ambiente;
    vec3 difusa;
    vec3 especular;
    float brilho;
};

#define MAX_LUZES_PONTUAIS 4

uniform vec3 posicaoObservador;
uniform Material material;
uniform LuzDirecional luzDirecional;
uniform LuzPontual luzesPontuais[MAX_LUZES_PONTUAIS];
uniform int numLuzesPontuais;

// Função para calcular contribuição de luz direcional
vec3 calcularLuzDirecional(LuzDirecional luz, vec3 normal, vec3 direcaoVisao) {
    vec3 direcaoLuz = normalize(-luz.direcao);
    
    // Componente difusa
    float diferencaDifusa = max(dot(normal, direcaoLuz), 0.0);
    
    // Componente especular (Blinn-Phong)
    vec3 direcaoMeio = normalize(direcaoLuz + direcaoVisao);
    float especular = pow(max(dot(normal, direcaoMeio), 0.0), material.brilho);
    
    // Combinar componentes
    vec3 ambiente = luz.ambiente * material.ambiente;
    vec3 difusa = luz.difusa * diferencaDifusa * material.difusa;
    vec3 especularFinal = luz.especular * especular * material.especular;
    
    return (ambiente + difusa + especularFinal);
}

// Função para calcular contribuição de luz pontual
vec3 calcularLuzPontual(LuzPontual luz, vec3 normal, vec3 posicaoFrag, vec3 direcaoVisao) {
    vec3 direcaoLuz = normalize(luz.posicao - posicaoFrag);
    
    // Componente difusa
    float diferencaDifusa = max(dot(normal, direcaoLuz), 0.0);
    
    // Componente especular (Blinn-Phong)
    vec3 direcaoMeio = normalize(direcaoLuz + direcaoVisao);
    float especular = pow(max(dot(normal, direcaoMeio), 0.0), material.brilho);
    
    // Atenuação
    float distancia = length(luz.posicao - posicaoFrag);
    float atenuacao = 1.0 / (luz.constante + luz.linear * distancia + 
                            luz.quadratica * (distancia * distancia));
    
    // Combinar componentes
    vec3 ambiente = luz.ambiente * material.ambiente;
    vec3 difusa = luz.difusa * diferencaDifusa * material.difusa;
    vec3 especularFinal = luz.especular * especular * material.especular;
    
    ambiente *= atenuacao;
    difusa *= atenuacao;
    especularFinal *= atenuacao;
    
    return (ambiente + difusa + especularFinal);
}

void main() {
    // Normalizar vetor normal
    vec3 normal = normalize(normalFragmento);
    vec3 direcaoVisao = normalize(posicaoObservador - posicaoFragmento);
    
    // Inicializar resultado com luz direcional
    vec3 resultado = calcularLuzDirecional(luzDirecional, normal, direcaoVisao);
    
    // Adicionar contribuição de luzes pontuais
    for (int i = 0; i < numLuzesPontuais; i++) {
        resultado += calcularLuzPontual(luzesPontuais[i], normal, posicaoFragmento, direcaoVisao);
    }
    
    corFinal = vec4(resultado, 1.0);
}

