# Componentes de la escena

`20271.cpp` inicializa la ventana y OpenGL, crea los componentes y coordina el ciclo
de dibujo. Cada integrante puede trabajar en un componente sin editar ese ciclo.
No se crearon ni modificaron encabezados `.h`.

| Archivo | Responsabilidad y lugares para editar |
| --- | --- |
| `Tumbas.cpp` | Cinco rutas de modelos en `cargarModelos()`. Zonas, filas, columnas, tipos, escala y pivote en `configurarDistribucion()`. Dibujo de las 72 instancias en `dibujar()`. |
| `Arboles.cpp` | Ruta del ahuehuete en el constructor. Diez posiciones, escalas y rotaciones en `configurarDistribucion()`. Pivote en `agregarArbol()`. |
| `Piso.cpp` | Ruta del plano en el constructor. Posicion y escala recibidas por el constructor y por `configurar()`. |
| `Iluminacion.cpp` | Luz nocturna en `configurar()`. Parametros de las dos luces puntuales y del reflector en `aplicar()`. |
| `ControlCamara.cpp` | Posicion inicial, velocidad, teclado, mouse, zoom, vista y proyeccion. Reutiliza la clase `Camera` existente. |
| `Fondo.cpp` | Rutas de las seis imagenes del cielo, shader del skybox y su dibujo. Reutiliza la clase `Skybox` existente. |

## Cargar, configurar y dibujar

1. Crear los objetos **despues** de inicializar el contexto y GLAD.
2. Los constructores cargan los recursos y preparan las transformaciones una vez,
   antes del ciclo. Los modelos pertenecen a su componente; las instancias los comparten.
3. Dentro del ciclo, activar `staticShader` y establecer vista, proyeccion e iluminacion.
4. Llamar a `piso.dibujar()`, `arboles.dibujar()` y `tumbas.dibujar()`.

```cpp
Fondo fondo;
Piso piso(glm::vec3(0.0f, alturaPiso, 0.0f), escalaPlano);
Arboles arboles(escalaPlano, alturaPiso);
Tumbas tumbas(escalaPlano, alturaPiso);
```

`Iluminacion` tambien aplica su configuracion inicial en el constructor.
`ControlCamara` ya inicializa su posicion y velocidad mediante su constructor.
No se necesitan llamadas posteriores a metodos de carga o configuracion inicial.

Las matrices de objetos estaticos se calculan durante la construccion. Si se modifica
la distribucion en ejecucion, volver a llamar a `configurarDistribucion()`; reemplaza
la lista anterior sin recargar los modelos. No hacerlo cada cuadro. `configurar()`
del piso y de la iluminacion sigue disponible para cambios posteriores. La carga
de las tumbas queda en un metodo privado que solamente llama su constructor.

El orden de dibujo se conserva: fondo, piso, arboles y tumbas. Arboles y tumbas
activan blending con `GL_SRC_ALPHA` / `GL_ONE_MINUS_SRC_ALPHA` y lo desactivan al
terminar. `Fondo` activa su propio shader; despues el main activa el de modelos.

## Coordenadas

En `20271.cpp` se definen una vez `escalaPlano = 15` y `alturaPiso = -1.75`.
Ambos se pasan al piso, los arboles y las tumbas. Las coordenadas del plano `(x, y)`
se convierten a coordenadas del mundo `(x * escalaPlano, alturaPiso, -y * escalaPlano)`.
El pivote se corrige en coordenadas locales; conservar el orden de las operaciones
de las matrices para no desplazar los modelos.

Para agregar otro arbol desde el main, despues de configurar la distribucion:

```cpp
// Posicion del mundo, escala y rotacion en grados sobre el eje Y.
arboles.agregarArbol(glm::vec3(100.0f, alturaPiso, 80.0f), 3.5f, 45.0f);
```

Para cambiar la distribucion de tumbas, editar los arreglos de `Tumbas.cpp`.
Los tipos van de 0 a 4, correspondientes a `tumba-1.obj` a `tumba-5.obj`.

## Controles

- W, A, S, D: mover la camara con `deltaTime`.
- Mouse: orientar la camara. Rueda: zoom.
- M / N: mover la primera luz puntual en X. Sus colores siguen en cero como en
  la escena base; para verla, cambiar sus componentes de color en `Iluminacion.cpp`.
- Escape: cerrar.

Los callbacks pequenos de `20271.cpp` delegan en los componentes usando
`glfwSetWindowUserPointer`, sin variables globales de camara o iluminacion.
La proyeccion usa el tamano actual del framebuffer y el dibujo se pausa al minimizar.

## Compilacion sin encabezados nuevos

Los seis `.cpp` se incluyen directamente desde `20271.cpp`. Se trata de **una sola
unidad de compilacion**, una simplificacion deliberada para este proyecto.

El `.vcxproj` los muestra en Visual Studio con `ExcludedFromBuild=true` para todas
las configuraciones, dentro del filtro **Archivos de origen / Componentes**.
No activar su compilacion independiente: se duplicarian las definiciones. Para un
nuevo componente, incluirlo una vez en el main y excluirlo igualmente de compilacion.
`STB_IMAGE_IMPLEMENTATION` debe permanecer solamente en `20271.cpp`.

Se retiraron del main los bloques inactivos de animacion y primitivas y sus shaders
sin objetos asociados. Los shaders y encabezados existentes siguen en el proyecto
para poder usarlos cuando se agreguen esos objetos. No se modificaron assets.
