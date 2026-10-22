/*---------------------------------------------------------*/
/* ----------------   Proyecto Final  --------------------------*/
/*-----------------    2027-1   ---------------------------*/
/*------------------- Alumnos:   ---------------------*/
/*------------- * Daniel Esteban Alcantara Paleo  ---------------*/
/*------------- * Emiliano  ---------------*/

#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// stb_image se implementa una sola vez, antes de incluir los componentes.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <shader_m.h>
#include <iostream>

// Estos .cpp contienen las clases completas y no se compilan por separado.
#include "componentes/Tumbas.cpp"
#include "componentes/Arboles.cpp"
#include "componentes/Piso.cpp"
#include "componentes/Iluminacion.cpp"
#include "componentes/ControlCamara.cpp"
#include "componentes/Fondo.cpp"

// Los callbacks acceden a los objetos del main mediante la ventana, sin globals.
struct ControlesVentana {
    ControlCamara camara;
    Iluminacion iluminacion;
    int ancho = 800;
    int alto = 600;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* controles = static_cast<ControlesVentana*>(glfwGetWindowUserPointer(window));
    controles->ancho = width;
    controles->alto = height;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto* controles = static_cast<ControlesVentana*>(glfwGetWindowUserPointer(window));
    controles->camara.procesarMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* controles = static_cast<ControlesVentana*>(glfwGetWindowUserPointer(window));
    controles->camara.procesarScroll(yoffset);
}

void my_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;
    auto* controles = static_cast<ControlesVentana*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_M) controles->iluminacion.moverLuzPrincipal(1.0f);
    if (key == GLFW_KEY_N) controles->iluminacion.moverLuzPrincipal(-1.0f);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const int ancho = mode ? mode->width : 800;
    const int alto = mode ? mode->height - 80 : 600;
    GLFWwindow* window = glfwCreateWindow(ancho, alto, "Pratica X 2027-1", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Los objetos se crean con el contexto listo y salen de alcance antes del cierre.
    {
        ControlesVentana controles;
        glfwSetWindowUserPointer(window, &controles);
        glfwGetFramebufferSize(window, &controles.ancho, &controles.alto);
        glViewport(0, 0, controles.ancho, controles.alto);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, my_input);

        // Conversion comun del plano al mundo, utilizada por los tres grupos.
        const float escalaPlano = 15.0f;
        const float alturaPiso = -1.75f;

        // Cada constructor carga sus recursos y deja el componente listo para dibujar.
        Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
        Fondo fondo;
        Piso piso(glm::vec3(0.0f, alturaPiso, 0.0f), escalaPlano);
        Arboles arboles(escalaPlano, alturaPiso);
        Tumbas tumbas(escalaPlano, alturaPiso);

        const int FPS = 60;
        const Uint64 tiempoCuadro = 1000 / FPS;
        double ultimoCuadro = glfwGetTime();

        
        while (!glfwWindowShouldClose(window)) {
            // Al minimizar no se calcula una proyeccion con altura cero.
            if (controles.ancho <= 0 || controles.alto <= 0) {
                glfwWaitEvents();
                ultimoCuadro = glfwGetTime();
                continue;
            }

            const Uint64 inicioCuadro = SDL_GetTicks();
            const double ahora = glfwGetTime();
            const float deltaTime = static_cast<float>(ahora - ultimoCuadro);
            ultimoCuadro = ahora;
            controles.camara.procesarTeclado(window, deltaTime);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            const glm::mat4 vista = controles.camara.obtenerVista();
            const glm::mat4 proyeccion = controles.camara.obtenerProyeccion(
                controles.ancho, controles.alto);

            // Se conserva el orden original: fondo, piso, arboles y tumbas.
            fondo.dibujar(vista, proyeccion, controles.camara.obtenerCamara());
            staticShader.use();
            staticShader.setMat4("view", vista);
            staticShader.setMat4("projection", proyeccion);
            controles.iluminacion.aplicar(staticShader, controles.camara.obtenerPosicion());
            piso.dibujar(staticShader);
            arboles.dibujar(staticShader);
            tumbas.dibujar(staticShader);

            const Uint64 tiempoRender = SDL_GetTicks() - inicioCuadro;
            if (tiempoRender < tiempoCuadro) {
                SDL_Delay(static_cast<Uint32>(tiempoCuadro - tiempoRender));
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwSetWindowUserPointer(window, nullptr);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
