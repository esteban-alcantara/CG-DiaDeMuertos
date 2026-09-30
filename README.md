# CG - Día de Muertos

Proyecto final de la materia Computación Gráfica e Interacción Humano Computadora en el semestre 2027-1.

El repositorio contiene el código, los shaders y las dependencias. Los modelos y las texturas se descargan por separado desde Google Drive.

## Integrantes

- Alcantara Paleo Daniel Esteban
- Guevara Chávez Emiliano

## Requisitos tecnicos

- Windows de 64 bits y Git.
- Visual Studio 2022
- 3ds Max 2027
- Controladores gráficos con soporte para OpenGL.
- Acceso a la carpeta compartida de assets.

Los headers, las bibliotecas `.lib` y las DLL se incluyen en el proyecto.

## 1. Clonar el repositorio

En una terminal:

```powershell
git clone https://github.com/esteban-alcantara/CG-DiaDeMuertos.git
cd CG-DiaDeMuertos
```

## 2. Descargar y descomprimir los assets

**Enlace de Google Drive: .**
https://drive.google.com/drive/folders/1pBlbdbgmFz7FENy4kXXCv_PAak6dVKhS?usp=sharing

Solicitar el acceso a la carpeta compartida.

1. Descarga las carpetas **resources** y **Texturas** desde Drive.
2. Si se descargan como archivos ZIP, descomprímelos.
3. Copia las carpetas `resources` y `Texturas` dentro de la carpeta interna /CG-DiaDeMuertos`.
4. Conserva las subcarpetas, los nombres y los archivos de materiales `.mtl` junto con sus modelos y texturas.

Evita que la extracción cree niveles adicionales como `resources/resources/` o `Texturas/Texturas/`. Por ejemplo, Aquaman debe estar en `resources/objects/Aquaman/`.

## 4. Compilar y ejecutar

1. Compila la solución.
2. En Visual Studio presiona **F5** para ejecutar con el depurador o **Ctrl + F5** para ejecutar sin depuración.

## Trabajo colaborativo y assets

- Versiona el código, los shaders y los archivos compartidos de configuración en GitHub.
- Comparte los modelos y las texturas mediante Drive, conservando su estructura de carpetas.
- Después de actualizar assets en Drive, avisa al equipo para que descargue los archivos actualizados. `git pull` no descarga assets de Drive.
- `resources/`, `Texturas/`, la carpeta opcional `assets/`, la caché de Visual Studio y las salidas de compilación están excluidas por `.gitignore`.
- Para actualizar el código local, guarda primero tus cambios y ejecuta `git pull` desde la raíz del repositorio.
