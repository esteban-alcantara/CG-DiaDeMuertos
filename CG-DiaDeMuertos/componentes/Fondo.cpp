// Se incluye desde 20271.cpp; no se compila por separado.
#pragma once

#include <shader_m.h>
#include <Skybox.h>
#include <vector>
#include <string>

class Fondo {
public:
    Fondo()
        : shader("Shaders/skybox.vs", "Shaders/skybox.fs"),
          skybox(std::vector<std::string>{
            "resources/skybox/right.png",
            "resources/skybox/left.png",
            "resources/skybox/top.png",
            "resources/skybox/bottom.png",
            "resources/skybox/front.png",
            "resources/skybox/back.png"
          }) {
        shader.use();
        shader.setInt("skybox", 0);
    }

    void dibujar(const glm::mat4& vista, const glm::mat4& proyeccion, Camera& camara) {
        shader.use();
        skybox.Draw(shader, vista, proyeccion, camara);
    }

private:
    Shader shader;
    Skybox skybox;
};
