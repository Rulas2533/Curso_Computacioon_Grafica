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
	//Agregar modelos nuevos
	Model eg1((char*)"Models/eg1.obj");
	Model eg2((char*)"Models/eg2.obj");
	Model eg3((char*)"Models/eg3.obj");
	Model eg4((char*)"Models/eg4.obj");
	Model eg5((char*)"Models/eg5.obj");
	Model eg6((char*)"Models/eg6.obj");
	Model eg7((char*)"Models/eg7.obj");
	Model eg8((char*)"Models/eg8.obj");
	Model eg9((char*)"Models/eg9.obj");
	Model eg10((char*)"Models/eg10.obj");
	Model eg11((char*)"Models/eg11.obj");
	Model eg12((char*)"Models/eg12.obj");
	Model eg13((char*)"Models/eg13.obj");
	Model eg14((char*)"Models/eg14.obj");
	Model eg15((char*)"Models/eg15.obj");
	Model eg16((char*)"Models/eg16.obj");
	Model eg17((char*)"Models/eg17.obj");
	Model eg18((char*)"Models/eg18.obj");
	Model eg19((char*)"Models/eg19.obj");
	Model eg20((char*)"Models/eg20.obj");
	Model eg21((char*)"Models/eg21.obj");
	Model eg22((char*)"Models/eg22.obj");
	Model esc1((char*)"Models/esc1.obj");
	Model esc2((char*)"Models/esc2.obj");
	Model esc3((char*)"Models/esc3.obj");
	Model esc4((char*)"Models/esc4.obj");
	Model esc5((char*)"Models/esc5.obj");
	Model esc6((char*)"Models/esc6.obj");
	Model esc7((char*)"Models/esc7.obj");
	Model esc8((char*)"Models/esc8.obj");
	Model esc9((char*)"Models/esc9.obj");
	Model esc10((char*)"Models/esc10.obj");
	Model esc11((char*)"Models/esc11.obj");
	Model esc12((char*)"Models/esc12.obj");
	Model esc13((char*)"Models/esc13.obj");
	Model esc14((char*)"Models/esc14.obj");
	Model esc15((char*)"Models/esc15.obj");
	Model esc16((char*)"Models/esc16.obj");
	Model esc17((char*)"Models/esc17.obj");
	Model pir((char*)"Models/pir.obj");
	Model puertas1((char*)"Models/puertas1.obj");
	Model puertas2((char*)"Models/puertas2.obj");
	Model puertas3((char*)"Models/puertas3.obj");
	Model puertas4((char*)"Models/puertas4.obj");
	Model sala1((char*)"Models/sala1.obj");
	Model sala2((char*)"Models/sala2.obj");
	Model sala3((char*)"Models/sala3.obj");
	Model sala4((char*)"Models/sala4.obj");
	//PT2
	Model ac1((char*)"Models/ac1.obj");
	Model ac2((char*)"Models/ac2.obj");
	Model ac3((char*)"Models/ac3.obj");
	Model ac4((char*)"Models/ac4.obj");
	Model ac5((char*)"Models/ac5.obj");
	Model ac6((char*)"Models/ac6.obj");
	Model ac7((char*)"Models/ac7.obj");
	Model ac8((char*)"Models/ac8.obj");
	Model ac9((char*)"Models/ac9.obj");
	Model ac10((char*)"Models/ac10.obj");
	Model ac11((char*)"Models/ac11.obj");
	Model ac12((char*)"Models/ac12.obj");
	Model ac13((char*)"Models/ac13.obj");
	Model ac14((char*)"Models/ac14.obj");
	Model ac15((char*)"Models/ac15.obj");
	Model ac16((char*)"Models/ac16.obj");
	Model ac17((char*)"Models/ac17.obj");
	Model ac18((char*)"Models/ac18.obj");
	Model ac19((char*)"Models/ac19.obj");
	Model ac20((char*)"Models/ac20.obj");
	Model ac21((char*)"Models/ac21.obj");
	Model ac22((char*)"Models/ac22.obj");
	Model ac23((char*)"Models/ac23.obj");
	Model ac24((char*)"Models/ac24.obj");
	Model ac25((char*)"Models/ac25.obj");
	Model ac26((char*)"Models/ac26.obj");
	Model ac27((char*)"Models/ac27.obj");
	Model ac28((char*)"Models/ac28.obj");
	Model cont((char*)"Models/cont.obj");
	Model cont1((char*)"Models/cont1.obj");
	Model cont2((char*)"Models/cont2.obj");
	Model cont3((char*)"Models/cont3.obj");
	Model cont4((char*)"Models/cont4.obj");
	Model cont5((char*)"Models/cont5.obj");
	Model cont6((char*)"Models/cont6.obj");
	Model cont7((char*)"Models/cont7.obj");
	Model cont8((char*)"Models/cont8.obj");
	Model cont9((char*)"Models/cont9.obj");
	Model cont10((char*)"Models/cont10.obj");
	Model cont11((char*)"Models/cont11.obj");
	Model cont12((char*)"Models/cont12.obj");
	Model cont13((char*)"Models/cont13.obj");
	Model cont14((char*)"Models/cont14.obj");
	Model cont15((char*)"Models/cont15.obj");
	Model cont16((char*)"Models/cont16.obj");
	Model cont17((char*)"Models/cont17.obj");
	Model cont18((char*)"Models/cont18.obj");
	Model cont19((char*)"Models/cont19.obj");
	Model cont20((char*)"Models/cont20.obj");
	Model cont21((char*)"Models/cont21.obj");
	Model cont22((char*)"Models/cont22.obj");
	Model cont23((char*)"Models/cont23.obj");
	Model cont24((char*)"Models/cont24.obj");
	Model cont25((char*)"Models/cont25.obj");
	Model cont26((char*)"Models/cont26.obj");
	Model cont27((char*)"Models/cont27.obj");
	Model cont28((char*)"Models/cont28.obj");
	Model cont29((char*)"Models/cont29.obj");
	Model cont30((char*)"Models/cont30.obj");
	//mete más antes de esta linea

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
		// 
		// Dibujar nuevos modelos - agregar aquí
		// En el bucle de renderizado (después del comentario "Dibujar nuevos modelos - agregar aquí")
// Dibujar todos los modelos eg (engranajes)
		glm::mat4 modelEg1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg1));
		eg1.Draw(lightingShader);

		glm::mat4 modelEg2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg2));
		eg2.Draw(lightingShader);

		glm::mat4 modelEg3 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg3));
		eg3.Draw(lightingShader);

		glm::mat4 modelEg4 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg4));
		eg4.Draw(lightingShader);

		glm::mat4 modelEg5 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg5));
		eg5.Draw(lightingShader);

		glm::mat4 modelEg6 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg6));
		eg6.Draw(lightingShader);

		glm::mat4 modelEg7 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg7));
		eg7.Draw(lightingShader);

		glm::mat4 modelEg8 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg8));
		eg8.Draw(lightingShader);

		glm::mat4 modelEg9 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg9));
		eg9.Draw(lightingShader);

		glm::mat4 modelEg10 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg10));
		eg10.Draw(lightingShader);

		glm::mat4 modelEg11 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg11));
		eg11.Draw(lightingShader);

		glm::mat4 modelEg12 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg12));
		eg12.Draw(lightingShader);

		glm::mat4 modelEg13 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg13));
		eg13.Draw(lightingShader);

		glm::mat4 modelEg14 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg14));
		eg14.Draw(lightingShader);

		glm::mat4 modelEg15 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg15));
		eg15.Draw(lightingShader);

		glm::mat4 modelEg16 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg16));
		eg16.Draw(lightingShader);

		glm::mat4 modelEg17 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg17));
		eg17.Draw(lightingShader);

		glm::mat4 modelEg18 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg18));
		eg18.Draw(lightingShader);

		glm::mat4 modelEg19 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg19));
		eg19.Draw(lightingShader);

		glm::mat4 modelEg20 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg20));
		eg20.Draw(lightingShader);

		glm::mat4 modelEg21 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg21));
		eg21.Draw(lightingShader);

		glm::mat4 modelEg22 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEg22));
		eg22.Draw(lightingShader);

		// Dibujar modelos esc
		glm::mat4 modelEsc1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc1));
		esc1.Draw(lightingShader);

		glm::mat4 modelEsc2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc2));
		esc2.Draw(lightingShader);

		glm::mat4 modelEsc3 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc3));
		esc3.Draw(lightingShader);

		glm::mat4 modelEsc4 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc4));
		esc4.Draw(lightingShader);

		glm::mat4 modelEsc5 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc5));
		esc5.Draw(lightingShader);

		glm::mat4 modelEsc6 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc6));
		esc6.Draw(lightingShader);

		glm::mat4 modelEsc7 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc7));
		esc7.Draw(lightingShader);

		glm::mat4 modelEsc8 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc8));
		esc8.Draw(lightingShader);

		glm::mat4 modelEsc9 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc9));
		esc9.Draw(lightingShader);

		glm::mat4 modelEsc10 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc10));
		esc10.Draw(lightingShader);

		glm::mat4 modelEsc11 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc11));
		esc11.Draw(lightingShader);

		glm::mat4 modelEsc12 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc12));
		esc12.Draw(lightingShader);

		glm::mat4 modelEsc13 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc13));
		esc13.Draw(lightingShader);

		glm::mat4 modelEsc14 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc14));
		esc14.Draw(lightingShader);

		glm::mat4 modelEsc15 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc15));
		esc15.Draw(lightingShader);

		glm::mat4 modelEsc16 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc16));
		esc16.Draw(lightingShader);

		glm::mat4 modelEsc17 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEsc17));
		esc17.Draw(lightingShader);

		// Dibujar modelos restantes
		glm::mat4 modelPir = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPir));
		pir.Draw(lightingShader);

		glm::mat4 modelPuertas1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertas1));
		puertas1.Draw(lightingShader);

		glm::mat4 modelPuertas2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertas2));
		puertas2.Draw(lightingShader);

		glm::mat4 modelPuertas3 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertas3));
		puertas3.Draw(lightingShader);

		glm::mat4 modelPuertas4 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPuertas4));
		puertas4.Draw(lightingShader);

		glm::mat4 modelSala1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSala1));
		sala1.Draw(lightingShader);

		glm::mat4 modelSala2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSala2));
		sala2.Draw(lightingShader);

		glm::mat4 modelSala3 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSala3));
		sala3.Draw(lightingShader);

		glm::mat4 modelSala4 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSala4));
		sala4.Draw(lightingShader);
		//PARTE 2
		// Dibujar modelos ac
		glm::mat4 modelAc1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc1));
		ac1.Draw(lightingShader);

		glm::mat4 modelAc2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc2));
		ac2.Draw(lightingShader);

		glm::mat4 modelAc3 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc3));
		ac3.Draw(lightingShader);

		glm::mat4 modelAc4 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc4));
		ac4.Draw(lightingShader);

		glm::mat4 modelAc5 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc5));
		ac5.Draw(lightingShader);

		glm::mat4 modelAc6 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc6));
		ac6.Draw(lightingShader);

		glm::mat4 modelAc7 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc7));
		ac7.Draw(lightingShader);

		glm::mat4 modelAc8 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc8));
		ac8.Draw(lightingShader);

		glm::mat4 modelAc9 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc9));
		ac9.Draw(lightingShader);

		glm::mat4 modelAc10 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc10));
		ac10.Draw(lightingShader);

		glm::mat4 modelAc11 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc11));
		ac11.Draw(lightingShader);

		glm::mat4 modelAc12 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc12));
		ac12.Draw(lightingShader);

		glm::mat4 modelAc13 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc13));
		ac13.Draw(lightingShader);

		glm::mat4 modelAc14 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc14));
		ac14.Draw(lightingShader);

		glm::mat4 modelAc15 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc15));
		ac15.Draw(lightingShader);

		glm::mat4 modelAc16 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc16));
		ac16.Draw(lightingShader);

		glm::mat4 modelAc17 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc17));
		ac17.Draw(lightingShader);

		glm::mat4 modelAc18 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc18));
		ac18.Draw(lightingShader);

		glm::mat4 modelAc19 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc19));
		ac19.Draw(lightingShader);

		glm::mat4 modelAc20 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc20));
		ac20.Draw(lightingShader);

		glm::mat4 modelAc21 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc21));
		ac21.Draw(lightingShader);

		glm::mat4 modelAc22 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc22));
		ac22.Draw(lightingShader);

		glm::mat4 modelAc23 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc23));
		ac23.Draw(lightingShader);

		glm::mat4 modelAc24 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc24));
		ac24.Draw(lightingShader);

		glm::mat4 modelAc25 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc25));
		ac25.Draw(lightingShader);

		glm::mat4 modelAc26 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc26));
		ac26.Draw(lightingShader);

		glm::mat4 modelAc27 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc27));
		ac27.Draw(lightingShader);

		glm::mat4 modelAc28 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAc28));
		ac28.Draw(lightingShader);

		// Dibujar modelos cont
		glm::mat4 modelCont = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont));
		cont.Draw(lightingShader);

		glm::mat4 modelCont1 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont1));
		cont1.Draw(lightingShader);

		glm::mat4 modelCont2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont2));
		cont2.Draw(lightingShader);

		glm::mat4 modelCont3 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont3));
		cont3.Draw(lightingShader);

		glm::mat4 modelCont4 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont4));
		cont4.Draw(lightingShader);

		glm::mat4 modelCont5 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont5));
		cont5.Draw(lightingShader);

		glm::mat4 modelCont6 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont6));
		cont6.Draw(lightingShader);

		glm::mat4 modelCont7 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont7));
		cont7.Draw(lightingShader);

		glm::mat4 modelCont8 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont8));
		cont8.Draw(lightingShader);

		glm::mat4 modelCont9 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont9));
		cont9.Draw(lightingShader);

		glm::mat4 modelCont10 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont10));
		cont10.Draw(lightingShader);

		glm::mat4 modelCont11 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont11));
		cont11.Draw(lightingShader);

		glm::mat4 modelCont12 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont12));
		cont12.Draw(lightingShader);

		glm::mat4 modelCont13 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont13));
		cont13.Draw(lightingShader);

		glm::mat4 modelCont14 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont14));
		cont14.Draw(lightingShader);

		glm::mat4 modelCont15 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont15));
		cont15.Draw(lightingShader);

		glm::mat4 modelCont16 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont16));
		cont16.Draw(lightingShader);

		glm::mat4 modelCont17 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont17));
		cont17.Draw(lightingShader);

		glm::mat4 modelCont18 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont18));
		cont18.Draw(lightingShader);

		glm::mat4 modelCont19 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont19));
		cont19.Draw(lightingShader);

		glm::mat4 modelCont20 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont20));
		cont20.Draw(lightingShader);

		glm::mat4 modelCont21 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont21));
		cont21.Draw(lightingShader);

		glm::mat4 modelCont22 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont22));
		cont22.Draw(lightingShader);

		glm::mat4 modelCont23 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont23));
		cont23.Draw(lightingShader);

		glm::mat4 modelCont24 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont24));
		cont24.Draw(lightingShader);

		glm::mat4 modelCont25 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont25));
		cont25.Draw(lightingShader);

		glm::mat4 modelCont26 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont26));
		cont26.Draw(lightingShader);

		glm::mat4 modelCont27 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont27));
		cont27.Draw(lightingShader);

		glm::mat4 modelCont28 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont28));
		cont28.Draw(lightingShader);

		glm::mat4 modelCont29 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont29));
		cont29.Draw(lightingShader);

		glm::mat4 modelCont30 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCont30));
		cont30.Draw(lightingShader);

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