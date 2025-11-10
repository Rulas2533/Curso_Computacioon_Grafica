//Palafox Jimenez Raúl 
//422132844
//Previo12
//3 de Noviembre de 2025

#include <iostream>
#include <cmath>
#include <fstream>  // Para guardar archivos

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

// Variables para animación de manecillas
bool animacionActiva = false;
float anguloHorario = 0.0f;
float anguloMinutero = 0.0f;

// Variables para el péndulo
float anguloPendulo = 0.0f;
float amplitudPendulo = 5.0f; // Ángulo máximo de oscilación (grados)
bool penduloDerecha = true;
glm::vec3 pivotManecillas(0.460781f, 2.10701f, 0.006229f);
glm::vec3 pivotPendulo(0.295819f, 1.60216f, 0.006229f);

// Variables para engranajes
float anguloEng1 = 0.0f;
float anguloEng2 = 0.0f;
glm::vec3 pivotEng1(0.150284f, 1.14464f, -0.375564f);
glm::vec3 pivotEng2(0.151259f, 0.477735f, -0.321072f);

// Variables para campana y badajo
float anguloCampana = 0.0f;
float escalaCampana = 1.0f;
bool campanaSonando = false;
float tiempoCampana = 0.0f;
glm::vec3 pivotCampana(0.168226f, 2.9f, 0.0f);

// Traslación global para todos los modelos
glm::vec3 traslacionGlobal(-42.6714f, 0.58425f, -0.088454f);

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Reloj Animado - Palafox Jimenez Raul", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models - Reloj y sus componentes
	Model reloj((char*)"Models/relojp.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Pend((char*)"Models/pend.obj");
	Model min((char*)"Models/minutero.obj");
	Model hor((char*)"Models/horario.obj");
	Model Eng1((char*)"Models/eng1.obj");
	Model Eng2((char*)"Models/eng2.obj");
	Model Campana((char*)"Models/camp.obj");
	Model Badajo((char*)"Models/bad.obj");
	Model Sala1((char*)"Models/SalaPrueba.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		// Actualizar animación de manecillas, péndulo, engranajes y campana si está activa
		if (animacionActiva)
		{
			// Horario: más lento (30 grados por segundo)
			anguloHorario += 30.0f * deltaTime;
			// Minutero: más rápido (360 grados por segundo)
			anguloMinutero += 360.0f * deltaTime;

			// Mantener ángulos en rango [0, 360]
			if (anguloHorario >= 360.0f) anguloHorario -= 360.0f;
			if (anguloMinutero >= 360.0f) anguloMinutero -= 360.0f;

			// Animación del péndulo - oscilación suave en X (ida y vuelta)
			float oscilacion = sin(glfwGetTime() * 2.0f) * amplitudPendulo;
			anguloPendulo = oscilacion;

			// Animación de engranajes - relación 2:1 (eng1 más rápido)
			anguloEng1 += 180.0f * deltaTime;  // Engranaje pequeño más rápido
			anguloEng2 -= 90.0f * deltaTime;   // Engranaje grande más lento (sentido contrario)

			// Mantener ángulos en rango [0, 360]
			if (anguloEng1 >= 360.0f) anguloEng1 -= 360.0f;
			if (anguloEng2 <= -360.0f) anguloEng2 += 360.0f;

			// Animación de la campana - suena cada minuto (cada 360 grados del minutero
			// Animación de la campana - suena cada 5 segundos (para prueba)
			static float tiempoDesdeUltimaCampana = 0.0f;
			tiempoDesdeUltimaCampana += deltaTime;

			if (tiempoDesdeUltimaCampana >= 5.0f) { // Suena cada 5 segundos
				campanaSonando = true;
				tiempoCampana = 0.0f;
				tiempoDesdeUltimaCampana = 0.0f;
			}

			// Controlar animación de la campana
			if (campanaSonando) {
				tiempoCampana += deltaTime;

				// Duración total de la animación de la campana (2 segundos)
				if (tiempoCampana < 2.0f) {
					// Movimiento oscilatorio rápido (vibración) - MÁS NOTABLE
					anguloCampana = sin(tiempoCampana * 40.0f) * 8.0f; // 8 grados de amplitud

					// Efecto de escala (crece y encoge para simular vibración) - MÁS NOTABLE
					escalaCampana = 1.0f + sin(tiempoCampana * 30.0f) * 0.2f; // 20% de variación
				}
				else {
					// Terminar animación
					campanaSonando = false;
					anguloCampana = 0.0f;
					escalaCampana = 1.0f;
				}
			}
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		glm::mat4 view = camera.GetViewMatrix();
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Dibujar piso
		glm::mat4 modelPiso = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiso));
		Piso.Draw(lightingShader);

		// Dibujar sala
		glm::mat4 modelS1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelS1));
		Sala1.Draw(lightingShader);
		// Dibujar reloj principal
		glm::mat4 modelReloj = glm::mat4(1.0f);
		modelReloj = glm::translate(modelReloj, traslacionGlobal);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelReloj));
		reloj.Draw(lightingShader);

		// Dibujar péndulo con animación
		glm::mat4 modelPend = glm::mat4(1.0f);
		modelPend = glm::translate(modelPend, traslacionGlobal);

		// Aplicar rotación con pivote del péndulo - oscilación en eje X
		modelPend = glm::translate(modelPend, pivotPendulo);
		modelPend = glm::rotate(modelPend, glm::radians(anguloPendulo), glm::vec3(1.0f, 0.0f, 0.0f)); // Oscila en X
		modelPend = glm::translate(modelPend, -pivotPendulo);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPend));
		Pend.Draw(lightingShader);

		// Dibujar minutero con rotación y pivote ajustado
		glm::mat4 modelMin = glm::mat4(1.0f);
		modelMin = glm::translate(modelMin, traslacionGlobal);

		// Aplicar rotación con pivote corregido
		modelMin = glm::translate(modelMin, pivotManecillas);
		modelMin = glm::rotate(modelMin, glm::radians(anguloMinutero), glm::vec3(1.0f, 0.0f, 0.0f)); // Eje X
		modelMin = glm::translate(modelMin, -pivotManecillas);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMin));
		min.Draw(lightingShader);

		// Dibujar horario con rotación y pivote ajustado
		glm::mat4 modelHor = glm::mat4(1.0f);
		modelHor = glm::translate(modelHor, traslacionGlobal);

		// Aplicar rotación con pivote corregido
		modelHor = glm::translate(modelHor, pivotManecillas);
		modelHor = glm::rotate(modelHor, glm::radians(anguloHorario), glm::vec3(1.0f, 0.0f, 0.0f)); // Eje X
		modelHor = glm::translate(modelHor, -pivotManecillas);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHor));
		hor.Draw(lightingShader);

		// Dibujar engranaje 1 (pequeño) con animación
		glm::mat4 modelEng1 = glm::mat4(1.0f);
		modelEng1 = glm::translate(modelEng1, traslacionGlobal);

		// Aplicar rotación con pivote
		modelEng1 = glm::translate(modelEng1, pivotEng1);
		modelEng1 = glm::rotate(modelEng1, glm::radians(anguloEng1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en eje Z
		modelEng1 = glm::translate(modelEng1, -pivotEng1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEng1));
		Eng1.Draw(lightingShader);

		// Dibujar engranaje 2 (grande) con animación
		glm::mat4 modelEng2 = glm::mat4(1.0f);
		modelEng2 = glm::translate(modelEng2, traslacionGlobal);

		// Aplicar rotación con pivote
		modelEng2 = glm::translate(modelEng2, pivotEng2);
		modelEng2 = glm::rotate(modelEng2, glm::radians(anguloEng2), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en eje Z
		modelEng2 = glm::translate(modelEng2, -pivotEng2);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEng2));
		Eng2.Draw(lightingShader);

		// Dibujar campana con animación de vibración
		glm::mat4 modelCampana = glm::mat4(1.0f);
		modelCampana = glm::translate(modelCampana, traslacionGlobal);

		// Aplicar animación de vibración con pivote
		modelCampana = glm::translate(modelCampana, pivotCampana);
		modelCampana = glm::rotate(modelCampana, glm::radians(anguloCampana), glm::vec3(1.0f, 0.0f, 0.0f)); // Vibración en eje X
		modelCampana = glm::scale(modelCampana, glm::vec3(escalaCampana, escalaCampana, escalaCampana)); // Efecto de vibración con escala
		modelCampana = glm::translate(modelCampana, -pivotCampana);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCampana));
		Campana.Draw(lightingShader);

		// Dibujar badajo (se mueve con la campana)
		glm::mat4 modelBadajo = glm::mat4(1.0f);
		modelBadajo = glm::translate(modelBadajo, traslacionGlobal);

		// Aplicar misma animación que la campana
		modelBadajo = glm::translate(modelBadajo, pivotCampana);
		modelBadajo = glm::rotate(modelBadajo, glm::radians(anguloCampana), glm::vec3(1.0f, 0.0f, 0.0f)); // Vibración en eje X
		modelBadajo = glm::scale(modelBadajo, glm::vec3(escalaCampana, escalaCampana, escalaCampana)); // Misma escala que campana
		modelBadajo = glm::translate(modelBadajo, -pivotCampana);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBadajo));
		Badajo.Draw(lightingShader);
		glBindVertexArray(0);

		// Also draw the lamp object
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void DoMovement()
{
	// Camera controls only
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Tecla R para activar/desactivar animación de manecillas y péndulo
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		animacionActiva = !animacionActiva;
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}