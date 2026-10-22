// Se incluye desde 20271.cpp; no se compila por separado.
#pragma once

#include <vector>
#include <shader_m.h>
#include <model.h>

class Arboles {
public:
    Arboles(float escalaPlano, float alturaPiso)
        : modelo("resources/objects/ahuehuete/ahuehuete.obj") {
        configurarDistribucion(escalaPlano, alturaPiso);
    }

    void configurarDistribucion(float escalaPlano, float alturaPiso) {
        const glm::vec2 posiciones[10] = {
            {-16.75f,   7.5f}, {16.75f,   7.5f},
            {-16.75f, -18.5f}, {16.75f, -18.5f},
            { -9.25f,  14.0f}, { 9.25f,  14.0f},
            {  9.25f,  -5.5f}, {-2.25f, -10.5f},
            { -2.25f, -23.5f}, { 2.25f, -23.5f}
        };
        const float escalas[10] = {3.8f, 3.5f, 4.0f, 3.7f, 3.2f, 3.4f, 3.0f, 3.5f, 3.5f, 3.5f};
        const float rotaciones[10] = {0, 0, 0, 0, 0, 0, 0, 0, 45.0f, 30.0f};

        transformaciones.clear();
        transformaciones.reserve(10);
        for (int i = 0; i < 10; ++i) {
            // Misma conversion de coordenadas del plano que utiliza Tumbas.
            agregarArbol(glm::vec3(posiciones[i].x * escalaPlano,
                alturaPiso, -posiciones[i].y * escalaPlano), escalas[i], rotaciones[i]);
        }
    }

    // Posicion en coordenadas del mundo; rotacion alrededor de Y, en grados.
    void agregarArbol(const glm::vec3& posicion, float escala, float rotacion = 0.0f) {
        glm::mat4 matriz = glm::translate(glm::mat4(1.0f), posicion);
        matriz = glm::rotate(matriz, glm::radians(rotacion), glm::vec3(0, 1, 0));
        matriz = glm::scale(matriz, glm::vec3(escala));
        matriz = glm::translate(matriz, glm::vec3(0.1198f, -0.1083f, -0.15035f));
        transformaciones.push_back(matriz);
    }

    // Requiere el shader activo con vista, proyeccion e iluminacion configuradas.
    // Deja blending desactivado, igual que Tumbas.
    void dibujar(Shader& shader) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (const glm::mat4& matriz : transformaciones) {
            shader.setMat4("model", matriz);
            modelo.Draw(shader);
        }
        glDisable(GL_BLEND);
    }

private:
    Model modelo;
    std::vector<glm::mat4> transformaciones;
};
