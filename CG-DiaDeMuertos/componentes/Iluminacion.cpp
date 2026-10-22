// Se incluye desde 20271.cpp; no se compila por separado.
#pragma once

#include <shader_m.h>

class Iluminacion {
public:
    Iluminacion() {
        configurar();
    }

    void configurar() {
        lightPosition = glm::vec3(0.0f, 4.0f, -10.0f);
        lightDirection = glm::vec3(0.0f, -1.0f, -1.0f);
        lightColor = glm::vec3(0.40f, 0.46f, 0.60f);
        diffuseColor = lightColor * 0.35f;
        ambientColor = diffuseColor * 0.55f;
    }

    // Requiere el shader activo. Las luces puntuales y el reflector
    // conservan sus colores en cero, como en la escena original.
    void aplicar(Shader& shader, const glm::vec3& posicionCamara) {
        shader.setVec3("viewPos", posicionCamara);
        shader.setVec3("dirLight.direction", lightDirection);
        shader.setVec3("dirLight.ambient", ambientColor);
        shader.setVec3("dirLight.diffuse", diffuseColor);
        shader.setVec3("dirLight.specular", lightColor * 0.2f);

        shader.setVec3("pointLight[0].position", lightPosition);
        shader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setFloat("pointLight[0].constant", 0.08f);
        shader.setFloat("pointLight[0].linear", 0.009f);
        shader.setFloat("pointLight[0].quadratic", 0.032f);

        shader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
        shader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setFloat("pointLight[1].constant", 1.0f);
        shader.setFloat("pointLight[1].linear", 0.009f);
        shader.setFloat("pointLight[1].quadratic", 0.032f);

        shader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
        shader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
        shader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
        shader.setFloat("spotLight[0].constant", 1.0f);
        shader.setFloat("spotLight[0].linear", 0.0009f);
        shader.setFloat("spotLight[0].quadratic", 0.0005f);

        shader.setFloat("material_shininess", 32.0f);
    }

    void moverLuzPrincipal(float desplazamientoX) {
        lightPosition.x += desplazamientoX;
    }

private:
    glm::vec3 lightPosition;
    glm::vec3 lightDirection;
    glm::vec3 lightColor;
    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
};
