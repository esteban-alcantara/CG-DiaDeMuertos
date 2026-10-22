// Se incluye desde 20271.cpp; no se compila por separado.
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <shader_m.h>
#include <model.h>

class Tumbas {
public:
    Tumbas(float escalaPlano, float alturaPiso) {
        cargarModelos();
        configurarDistribucion(escalaPlano, alturaPiso);
    }

    // Coordenadas del plano: (x, y) se convierte a (x * escala, altura, -y * escala).
    void configurarDistribucion(float escalaPlano, float alturaPiso) {
        const glm::vec2 zonas[6] = {
            {-9.25f,   7.5f}, {9.25f,   7.5f}, // A, B
            {-9.25f,  -5.5f}, {9.25f,  -5.5f}, // C, D
            {-9.25f, -18.5f}, {9.25f, -18.5f}  // E, F
        };
        const float columnas[6] = {-6.25f, -3.75f, -1.25f, 1.25f, 3.75f, 6.25f};
        const float filas[2] = {-2.5f, 2.5f};
        const int distribucion[2][6] = {
            {0, 2, 4, 1, 3, 0},
            {3, 1, 0, 4, 2, 1}
        };

        tumbas.clear();
        tumbas.reserve(72);
        // Se dibujan todas las tumbas usando solamente cinco modelos compartidos.
        for (int zona = 0; zona < 6; ++zona) {
            for (int fila = 0; fila < 2; ++fila) {
                for (int columna = 0; columna < 6; ++columna) {
                    const float x = zonas[zona].x + columnas[columna];
                    const float y = zonas[zona].y + filas[fila];
                    const int tipo = (distribucion[fila][columna] + zona) % 5;
                    glm::mat4 matriz = glm::translate(glm::mat4(1.0f),
                        glm::vec3(x * escalaPlano, alturaPiso, -y * escalaPlano));
                    matriz = glm::scale(matriz, glm::vec3(1.5f));
                    // Correccion en coordenadas locales; conservar el orden T * S * T_pivote.
                    matriz = glm::translate(matriz, glm::vec3(-9.9706f, 0.0f, -20.0235f));
                    tumbas.push_back({tipo, matriz});
                }
            }
        }
    }

    // El main activa el shader y establece vista, proyeccion e iluminacion.
    // Este grupo usa blending y lo deja desactivado al terminar.
    void dibujar(Shader& shader) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (const Instancia& tumba : tumbas) {
            shader.setMat4("model", tumba.matriz);
            modelos[tumba.tipo]->Draw(shader);
        }
        glDisable(GL_BLEND);
    }

private:
    // Solo el constructor carga los cinco modelos compartidos.
    void cargarModelos() {
        for (int tipo = 0; tipo < 5; ++tipo) {
            const std::string ruta = "resources/objects/tumbas/tumba-"
                + std::to_string(tipo + 1) + ".obj";
            modelos[tipo].reset(new Model(ruta));
        }
    }

    struct Instancia {
        int tipo;
        glm::mat4 matriz;
    };

    std::unique_ptr<Model> modelos[5];
    std::vector<Instancia> tumbas;
};
