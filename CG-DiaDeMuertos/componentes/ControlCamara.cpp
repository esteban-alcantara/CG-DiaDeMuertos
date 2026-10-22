// Se incluye desde 20271.cpp; no se compila por separado.
#pragma once

#include <glfw3.h>
#include <camera.h>

class ControlCamara {
public:
    ControlCamara() : camara(glm::vec3(0.0f, 10.0f, 3.0f)) {
        camara.MovementSpeed = 60.0f;
    }

    void procesarTeclado(GLFWwindow* ventana, float deltaTime) {
        if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS)
            camara.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS)
            camara.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS)
            camara.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS)
            camara.ProcessKeyboard(RIGHT, deltaTime);
    }

    void procesarMouse(double x, double y) {
        if (primerMovimiento) {
            ultimoX = x;
            ultimoY = y;
            primerMovimiento = false;
        }
        const float desplazamientoX = static_cast<float>(x - ultimoX);
        const float desplazamientoY = static_cast<float>(ultimoY - y);
        ultimoX = x;
        ultimoY = y;
        camara.ProcessMouseMovement(desplazamientoX, desplazamientoY);
    }

    void procesarScroll(double desplazamiento) {
        camara.ProcessMouseScroll(static_cast<float>(desplazamiento));
    }

    glm::mat4 obtenerVista() {
        return camara.GetViewMatrix();
    }

    // El main evita renderizar cuando el framebuffer esta minimizado (altura cero).
    glm::mat4 obtenerProyeccion(int ancho, int alto) const {
        return glm::perspective(glm::radians(camara.Zoom),
            static_cast<float>(ancho) / static_cast<float>(alto), 0.1f, 10000.0f);
    }

    const glm::vec3& obtenerPosicion() const {
        return camara.Position;
    }

    // La clase Skybox existente recibe una Camera al dibujar.
    Camera& obtenerCamara() {
        return camara;
    }

private:
    Camera camara;
    double ultimoX = 0.0;
    double ultimoY = 0.0;
    bool primerMovimiento = true;
};
