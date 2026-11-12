/*---------------------------------------------------------*/
/* ----------------   Proyecto Final  --------------------------*/
/*-----------------    2027-1   ---------------------------*/
/*------------------- Alumnos:   ---------------------*/
/*------------- * Daniel Esteban Alcantara Paleo  ---------------*/
/*------------- * Emiliano  ---------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;


//Camera
Camera camera(glm::vec3(0.0f, 10.0f, 3.0f));
float MovementSpeed = 120.0f; // Unidades por segundo para la camara.
GLfloat lastX = SCR_WIDTH / 2.0f,
lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
//glm::vec3 lightColor = glm::vec3(0.40f, 0.46f, 0.60f); // Luz nocturna ligeramente azulada.
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.35f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.55f);

void animate(void)
{
	// Ejemplo de las practicas: inactivo mientras no haya objetos que lo utilicen.
	// 	if (play)
	// 	{
	// 		if (i_curr_steps >= i_max_steps) //end of animation between frames?
	// 		{
	// 			playIndex++;
	// 			if (playIndex > FrameIndex - 2)	//end of total animation?
	// 			{
	// 				std::cout << "Animation ended" << std::endl;
	// 				//printf("termina anim\n");
	// 				playIndex = 0;
	// 				play = false;
	// 			}
	// 			else //Next frame interpolations
	// 			{
	// 				i_curr_steps = 0; //Reset counter
	// 				//Interpolation
	// 				interpolation();
	// 			}
	// 		}
	// 		else
	// 		{
	// 			//Draw animation
	// 			posX += incX;
	// 			posY += incY;
	// 			posZ += incZ;
	//
	// 			rotRodIzq += rotRodIzqInc;
	// 			giroMonito += giroMonitoInc;
	//
	// 			i_curr_steps++;
	// 		}
	// 	}
	//
	// 	//Veh?culo
	// 	if (animacion)
	// 	{
	// 		movAuto_x += 3.0f;
	// 	}
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

int main() {
	// glfw: initialize and configure
	glfwInit();

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pratica X 2027-1", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	glEnable(GL_DEPTH_TEST);



	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//To use with skybox
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");	//To use with animated models 

	vector<std::string> faces{
		"resources/skybox/right.png",
		"resources/skybox/left.png",
		"resources/skybox/top.png",
		"resources/skybox/bottom.png",
		"resources/skybox/front.png",
		"resources/skybox/back.png"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/planoPrincipal/planoPrincipal.obj");

	Model ahuehuete("resources/objects/ahuehuete/ahuehuete.obj");

	Model tumba_1("resources/objects/tumbas/tumba-1.obj");
	Model tumba_2("resources/objects/tumbas/tumba-2.obj");
	Model tumba_3("resources/objects/tumbas/tumba-3.obj");
	Model tumba_4("resources/objects/tumbas/tumba-4.obj");
	Model tumba_5("resources/objects/tumbas/tumba-5.obj");

	Model muro("resources/objects/Muro/muro.obj");

	Model iglesia("resources/objects/iglesia/iglesia.obj");
	Model campanario("resources/objects/campanario/campanario.obj");
	Model paredder("resources/objects/paredder/paredder.obj");
	Model paredizq("resources/objects/paredizq/paredizq.obj");
	Model entrada("resources/objects/entrada/entrada.obj");

	Model poste("resources/objects/poste/scene.gltf");
	Model papelpicado("resources/objects/papelpicado/scene.gltf");

	Model catrina("resources/objects/catrina/scene.gltf");

	

	// Reutilizar los cinco modelos para dibujar las 72 tumbas.
	Model* modelosTumbas[5] = { &tumba_1, &tumba_2, &tumba_3, &tumba_4, &tumba_5 };


	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	// render loop
	// -----------
	camera.MovementSpeed = MovementSpeed;
	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		const Uint64 frameStart = SDL_GetTicks();
		const double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame; // Segundos entre cuadros completos.
		lastFrame = currentFrame;

		// input
		// -----
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, (float)deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, (float)deltaTime);

		animate();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", ambientColor);
		staticShader.setVec3("dirLight.diffuse", diffuseColor);
		staticShader.setVec3("dirLight.specular", lightColor * 0.2f);

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);


		glm::mat4 tmp = glm::mat4(1.0f);
		projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Setup shader for primitives
		myShader.use();
		// view/projection transformations
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
		viewOp = camera.GetViewMatrix();
		// pass them to the shaders
		//myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/

		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		// ------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(15.0f));
		staticShader.setMat4("model", modelOp);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------




		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const glm::vec2 posicionesArboles[10] = {
			{-16.75f,   7.5f},
			{ 16.75f,   7.5f},
			{-16.75f, -18.5f},
			{ 16.75f, -18.5f},
			{ -9.25f,  14.0f},
			{  9.25f,  14.0f},
			{  9.25f,  -5.5f},
			{ -2.25f, -10.5f},
			{ -2.25f, -23.5f},
			{  2.25f, -23.5f}
		};
		// Alturas aproximadas de los arboles
		const float escalasArboles[10] = { 3.8f, 3.5f, 4.0f, 3.7f, 3.2f, 3.4f, 3.0f, 3.5f, 3.5f, 3.5f };

		for (int arbol = 0; arbol < 10; arbol++) {
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(
				posicionesArboles[arbol].x * 15.0f,
				-1.75f,
				-posicionesArboles[arbol].y * 15.0f));

			if (arbol == 8 || arbol == 9) {
				modelOp = glm::rotate(modelOp, glm::radians(arbol == 8 ? 45.0f : 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			modelOp = glm::scale(modelOp, glm::vec3(escalasArboles[arbol]));
			modelOp = glm::translate(modelOp, glm::vec3(0.1198f, -0.1083f, -0.15035f));
			staticShader.setMat4("model", modelOp);
			ahuehuete.Draw(staticShader);
		}
		glDisable(GL_BLEND);

		// -------------------------------------------------------------------------------------------------------------------------
		// Tumbas: 6 secciones, 2 filas y 6 columnas en cada seccion (72 tumbas).
		// -------------------------------------------------------------------------------------------------------------------------
		const glm::vec2 zonas[6] = {
			{-9.25f,   7.5f},  // A
			{ 9.25f,   7.5f},  // B
			{-9.25f,  -5.5f},  // C
			{ 9.25f,  -5.5f},  // D
			{-9.25f, -18.5f},  // E
			{ 9.25f, -18.5f}   // F
		};

		const float columnas[6] = { -6.25f, -3.75f, -1.25f, 1.25f, 3.75f, 6.25f };
		const float filas[2] = { -2.5f, 2.5f };

		// Indices 0..4 equivalen a tumba_1..tumba_5.
		const int distribucion[2][6] = {
			{ 0, 2, 4, 1, 3, 0 },
			{ 3, 1, 0, 4, 2, 1 }
		};

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int zona = 0; zona < 6; zona++) {
			for (int fila = 0; fila < 2; fila++) {
				for (int columna = 0; columna < 6; columna++) {
					const float x = zonas[zona].x + columnas[columna];
					const float y = zonas[zona].y + filas[fila];
					const int tipo = (distribucion[fila][columna] + zona) % 5;

					modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(x * 15.0f, -1.75f, -y * 15.0f));
					modelOp = glm::scale(modelOp, glm::vec3(1.5f));

					// Compensar el pivote local antes de escalar, sin cambiar la altura.
					modelOp = glm::translate(modelOp, glm::vec3(-9.9706f, 0.0f, -20.0235f));

					staticShader.setMat4("model", modelOp);
					modelosTumbas[tipo]->Draw(staticShader);
				}
			}
		}

		glDisable(GL_BLEND);

		//-------------------------------------------------------------------------------------
		// Muro 
		// ------------------------------------------------------------------------------------

		// 1. Parámetros del perímetro (ajusta estos límites según hasta dónde llega tu piso)
		const float minX = -14.50f;
		const float maxX = 20.45f;
		const float minZ = -27.85f;
		const float maxZ = 22.00f;

		// 2. Parámetros del modelo
		const float paso = 3.32f;        // Distancia entre cada muro 
		const float alturaMuro = -5.40f; // Altura a nivel del sueldo
		const glm::vec3 ajustePivote(0.0f, 0.0f, 0.0f); // Compensación de pivote local (como hiciste con las tumbas)

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// 3. Muros Superior e Inferior (Recorren el eje X)
		for (float x = minX-(paso + 1.0f); x <= maxX; x += paso) {
			// Borde Superior (z = maxZ)
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(x * 15.0f, alturaMuro, maxZ * 15.0f));
			modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación frontal
			modelOp = glm::translate(modelOp, ajustePivote);
			staticShader.setMat4("model", modelOp);
			muro.Draw(staticShader);

			// Borde Inferior (z = minZ)
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(x * 15.0f, alturaMuro, minZ * 15.0f));
			modelOp = glm::rotate(modelOp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotados para mirar hacia adentro
			modelOp = glm::translate(modelOp, ajustePivote);
			staticShader.setMat4("model", modelOp);
			muro.Draw(staticShader);
		}

		// 4. Muros Laterales Izquierdo y Derecho (Recorren el eje Z)
		// Usamos minZ + paso y maxZ - paso para que las esquinas no se empalmen con los muros superior/inferior
		for (float z = minZ- (paso + 1.0f); z <= maxZ; z += paso) {
			// Borde Izquierdo (x = minX)
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(minX * 15.0f, alturaMuro, z * 15.0f));
			modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotados 90 grados
			modelOp = glm::translate(modelOp, ajustePivote);
			staticShader.setMat4("model", modelOp);
			muro.Draw(staticShader);

			// Borde Derecho (x = maxX)
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(maxX * 15.0f, alturaMuro, z * 15.0f));
			modelOp = glm::rotate(modelOp, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotados -90 grados
			modelOp = glm::translate(modelOp, ajustePivote);
			staticShader.setMat4("model", modelOp);
			muro.Draw(staticShader);
		}

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(15.0f));
		staticShader.setMat4("model", modelOp);
		iglesia.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(15.0f));
		staticShader.setMat4("model", modelOp);
		campanario.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(15.0f));
		staticShader.setMat4("model", modelOp);
		paredder.Draw(staticShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -1.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(15.0f));
		staticShader.setMat4("model", modelOp);
		paredizq.Draw(staticShader);

		glDisable(GL_BLEND);

		//-------------------------------------------------------------------------------------
		// Modelo Catrina
		// -------------------

		staticShader.use();
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Posiciónala en el escenario
		// modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Descomentar si necesitas girarla
		modelOp = glm::scale(modelOp, glm::vec3(10.0f)); // Cambia este valor (ej. 0.1f o 10.0f) si el modelo sale muy grande o muy pequeña
		staticShader.setMat4("model", modelOp);
		catrina.Draw(staticShader);

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Postes + papel picado entre cada par
		// -------------------------------------------------------------------------------------------------------------------------
		// Convencion: (x, y) en unidades de plano y Z_mundo = -y * 15 (igual que tumbas y arboles).
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const float ESCALA_ESCENA = 15.0f;
		const float ALTURA_POSTE = -5.40f;
		const float ESCALA_POSTE = 10.5f;
		const float AJUSTE_YAW_POSTE = 0.0f;    // si la lampara mira al lado equivocado prueba 90, 180 o -90

		// El origen del modelo del poste no esta en su eje; este desplazamiento (unidades locales) lo centra.
		const float PIVOTE_POSTE_X = -0.79f;
		const float PIVOTE_POSTE_Y = -0.55f;

		// Geometria del escenario (unidades de plano)
		const float X_POSTE = 1.6f;      // distancia del eje del pasillo central (x = 0) a sus postes
		const float DY_CRUCE = 1.4f;      // distancia del eje de cada pasillo lateral a sus postes (y a donde empiezan/terminan los tramos del central)
		const float CRUCES_Y[2] = { 1.0f, -12.0f };  // ejes de los pasillos laterales
		const float EXTREMO_SUP_Y = 12.0f;     // extremos del pasillo central
		const float EXTREMO_INF_Y = -22.0f;
		const float EXTREMO_X = 16.8f;     // extremos de los pasillos laterales (a ambos lados)
		const int   PARES_POR_TRAMO = 2;               // pares en cada uno de los 3 tramos del central
		const int   PARES_LATERAL_POR_MITAD = 2;       // pares en cada mitad (izq/der) de cada pasillo lateral

		// Papel picado: medidas reales del modelo (3 banderas en fila sobre el eje X)
		const float PAPEL_LARGO_MODELO = 8.645f;   // largo total en X
		const float PAPEL_CENTRO_X = 0.8786f;  // centro en X (el origen del modelo no esta centrado)
		const float PAPEL_TOPE_Y = 2.066f;   // borde superior; de aqui "cuelga" del poste
		const float ALTURA_PAPEL = 75.0f;    // altura (sobre la base del poste) a la que se amarra el papel
		const float AJUSTE_YAW_PAPEL = 0.0f;

		auto dibujaPoste = [&](float x, float y, float yaw) {
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(x * 15.0f, ALTURA_POSTE, -y * 15.0f));
			modelOp = glm::rotate(modelOp, glm::radians(yaw + AJUSTE_YAW_POSTE), glm::vec3(0.0f, 1.0f, 0.0f));
			modelOp = glm::rotate(modelOp, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // modelo gltf Z-up
			modelOp = glm::scale(modelOp, glm::vec3(ESCALA_POSTE));
			modelOp = glm::translate(modelOp, glm::vec3(PIVOTE_POSTE_X, PIVOTE_POSTE_Y, 0.0f));
			staticShader.setMat4("model", modelOp);
			poste.Draw(staticShader);
			};

		// Papel picado de poste a poste: centrado en (xc, yc), cubre 'largo' unidades de plano completas.
		// yaw 0 = a lo largo de X (cruza el pasillo central), yaw 90 = a lo largo de Z (cruza un pasillo lateral).
		// Escala uniforme (sin deformar las banderas), centrado y colgando desde su borde superior.
		auto dibujaPapel = [&](float xc, float yc, float largo, float yaw) {
			const float s = (largo * ESCALA_ESCENA) / PAPEL_LARGO_MODELO;
			modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(xc * ESCALA_ESCENA, ALTURA_POSTE + ALTURA_PAPEL, -yc * ESCALA_ESCENA));
			modelOp = glm::rotate(modelOp, glm::radians(yaw + AJUSTE_YAW_PAPEL), glm::vec3(0.0f, 1.0f, 0.0f));
			modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 25.0f));
			modelOp = glm::translate(modelOp, glm::vec3(-PAPEL_CENTRO_X, -PAPEL_TOPE_Y, 0.0f));
			staticShader.setMat4("model", modelOp);
			papelpicado.Draw(staticShader);
			};

		// ---------- PASILLO CENTRAL: filas (cada fila = 1 par izquierda/derecha) ----------
		float filasY[3 * PARES_POR_TRAMO];       // PARES_POR_TRAMO filas en cada uno de los 3 tramos
		int nFilas = 0;
		// Tramos: de un limite a otro (los limites dejan libre la zona de cada interseccion).
		// Los pares quedan a la MISMA distancia entre si y respecto al inicio y al final del tramo.
		const float tramos[3][2] = {
			{ CRUCES_Y[0] + DY_CRUCE, EXTREMO_SUP_Y },                    // superior
			{ CRUCES_Y[1] + DY_CRUCE, CRUCES_Y[0] - DY_CRUCE },           // entre intersecciones
			{ EXTREMO_INF_Y,          CRUCES_Y[1] - DY_CRUCE }            // inferior
		};
		for (int t = 0; t < 3; t++)
			for (int k = 1; k <= PARES_POR_TRAMO; k++)
				filasY[nFilas++] = tramos[t][0] + (tramos[t][1] - tramos[t][0]) * k / (PARES_POR_TRAMO + 1);

		for (int i = 0; i < nFilas; i++) {
			dibujaPoste(-X_POSTE, filasY[i], 0.0f);                    // izquierdo, mira hacia +X
			dibujaPoste(X_POSTE, filasY[i], 180.0f);                    // derecho,   mira hacia -X
			dibujaPapel(0.0f, filasY[i], 2.0f * X_POSTE, 0.0f);          // papel a traves del pasillo central
		}

		// ---------- PASILLOS LATERALES: cada mitad (de la esquina al extremo) se parte en porciones iguales ----------
		for (int c = 0; c < 2; c++) {                                    // los 2 pasillos laterales
			for (int lado = -1; lado <= 1; lado += 2) {                  // -1 = mitad izquierda, +1 = mitad derecha
				for (int k = 1; k <= PARES_LATERAL_POR_MITAD; k++) {
					// centro de la porcion k, medido desde el pasillo central (X_POSTE) hasta el extremo
					const float x = lado * (X_POSTE + (EXTREMO_X - X_POSTE) * (k - 0.5f) / PARES_LATERAL_POR_MITAD);
					dibujaPoste(x, CRUCES_Y[c] + DY_CRUCE, -90.0f);          // mira hacia +Z (hacia el eje del pasillo)
					dibujaPoste(x, CRUCES_Y[c] - DY_CRUCE, 90.0f);          // mira hacia -Z
					dibujaPapel(x, CRUCES_Y[c], 2.0f * DY_CRUCE, 90.0f);     // papel a traves del pasillo lateral
				}
			}
		}

		glDisable(GL_BLEND);
	

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------


		// Limitar el framerate a 60
		const Uint64 renderTime = SDL_GetTicks() - frameStart;
		if (renderTime < LOOP_TIME)
		{
			SDL_Delay((Uint32)(LOOP_TIME - renderTime));
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	//skybox.Terminate();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);



	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}