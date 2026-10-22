// Se incluye desde 20271.cpp; no se compila por separado.
#pragma once

#include <shader_m.h>
#include <model.h>

class Piso {
public:
    Piso(const glm::vec3& posicion, float escala)
        : modelo("resources/objects/planoPrincipal/planoPrincipal.obj") {
        configurar(posicion, escala);
    }

    void configurar(const glm::vec3& posicion, float escala) {
        matriz = glm::translate(glm::mat4(1.0f), posicion);
        matriz = glm::scale(matriz, glm::vec3(escala));
    }

    void dibujar(Shader& shader) {
        shader.setMat4("model", matriz);
        modelo.Draw(shader);
    }

private:
    Model modelo;
    glm::mat4 matriz = glm::mat4(1.0f);
};
