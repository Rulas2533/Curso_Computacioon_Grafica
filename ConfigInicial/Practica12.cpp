//Palafox Jimenez Raúl 
//422132844
//Practica 12
//9 de Noviembre de 2025

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
void Animation();
void saveAnimationToFile();  // Nueva función para guardar animación
void loadAnimationFromFile(); // Nueva función para cargar animación
void sitDog(); // Nueva función para sentar al perro
void standDog(); // Nueva función para parar al perro
void moveBodyAndHeadUp(); // Mover torso y cabeza arriba
void moveBodyAndHeadDown(); // Mover torso y cabeza abajo

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
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
float bodyHeight = 0.0f; // Nueva variable para altura del cuerpo

//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame {
	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float HeadInc;
	float FLegs;        // Ángulo de patas delanteras
	float FLegsInc;     // Incremento de patas delanteras
	float RLegs;        // Ángulo de patas traseras  
	float RLegsInc;     // Incremento de patas traseras
	float tail;         // Ángulo de cola
	float tailInc;      // Incremento de cola
	float bodyHeight;   // Altura del cuerpo
	float bodyHeightInc; // Incremento de altura del cuerpo
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

// Función para mover torso y cabeza hacia arriba
void moveBodyAndHeadUp() {
	bodyHeight += 0.0001;  // Subir el torso
	head -= 0.005f;         // Bajar la cabeza (para mantener proporción)
}

// Función para mover torso y cabeza hacia abajo
void moveBodyAndHeadDown() {
	bodyHeight -= 0.0001;  // Bajar el torso
	head += 0.005f;         // Subir la cabeza (para mantener proporción)
}

// Función mejorada para sentar al perro
void sitDog() {
	FLegs = 45.0f;   // Patas delanteras dobladas
	RLegs = -60.0f;  // Patas traseras dobladas hacia atrás
	bodyHeight = -0.2f; // Cuerpo más bajo
	head = 15.0f;    // Cabeza levantada
	tail = -25.0f;   // Cola hacia abajo
}

// Función mejorada para parar al perro
void standDog() {
	FLegs = 0.0f;    // Patas delanteras rectas
	RLegs = 0.0f;    // Patas traseras rectas
	bodyHeight = 0.0f; // Cuerpo en altura normal
	head = 0.0f;     // Cabeza recta
	tail = 0.0f;     // Cola recta
}

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].FLegs = FLegs;  // Guardar patas delanteras
	KeyFrame[FrameIndex].RLegs = RLegs;  // Guardar patas traseras
	KeyFrame[FrameIndex].tail = tail;    // Guardar cola
	KeyFrame[FrameIndex].bodyHeight = bodyHeight; // Guardar altura del cuerpo

	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	rotDog = KeyFrame[0].rotDog;
	head = KeyFrame[0].head;
	FLegs = KeyFrame[0].FLegs;  // Resetear patas delanteras
	RLegs = KeyFrame[0].RLegs;  // Resetear patas traseras
	tail = KeyFrame[0].tail;    // Resetear cola
	bodyHeight = KeyFrame[0].bodyHeight; // Resetear altura del cuerpo
}

void interpolation(void)
{
	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].HeadInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
	KeyFrame[playIndex].FLegsInc = (KeyFrame[playIndex + 1].FLegs - KeyFrame[playIndex].FLegs) / i_max_steps;  // Interpolación patas delanteras
	KeyFrame[playIndex].RLegsInc = (KeyFrame[playIndex + 1].RLegs - KeyFrame[playIndex].RLegs) / i_max_steps;  // Interpolación patas traseras
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;      // Interpolación cola
	KeyFrame[playIndex].bodyHeightInc = (KeyFrame[playIndex + 1].bodyHeight - KeyFrame[playIndex].bodyHeight) / i_max_steps; // Interpolación altura cuerpo
}

// Función para guardar la animación en archivo
void saveAnimationToFile() {
	std::ofstream file("animacion_perro.txt");  // Ruta relativa
	if (file.is_open()) {
		file << FrameIndex << std::endl;
		for (int i = 0; i < FrameIndex; i++) {
			file << KeyFrame[i].dogPosX << " " << KeyFrame[i].dogPosY << " " << KeyFrame[i].dogPosZ << " "
				<< KeyFrame[i].rotDog << " " << KeyFrame[i].head << " "
				<< KeyFrame[i].FLegs << " " << KeyFrame[i].RLegs << " " << KeyFrame[i].tail << " "
				<< KeyFrame[i].bodyHeight << std::endl;
		}
		file.close();
		std::cout << "Animacion guardada en animacion_perro.txt - Frames: " << FrameIndex << std::endl;
	}
	else {
		std::cout << "Error al guardar la animacion" << std::endl;
	}
}

// Función para cargar la animación desde archivo
void loadAnimationFromFile() {
	std::ifstream file("animacion_perro.txt");  // Ruta relativa
	if (file.is_open()) {
		file >> FrameIndex;
		for (int i = 0; i < FrameIndex; i++) {
			file >> KeyFrame[i].dogPosX >> KeyFrame[i].dogPosY >> KeyFrame[i].dogPosZ
				>> KeyFrame[i].rotDog >> KeyFrame[i].head
				>> KeyFrame[i].FLegs >> KeyFrame[i].RLegs >> KeyFrame[i].tail
				>> KeyFrame[i].bodyHeight;
		}
		file.close();
		std::cout << "Animacion cargada desde animacion_perro.txt - Frames: " << FrameIndex << std::endl;
		// Actualizar la posición actual del perro con el primer frame
		resetElements();
	}
	else {
		std::cout << "No se encontro archivo de animacion" << std::endl;
	}
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Animacion Keyframes Palafox Jimenez Raul", nullptr, nullptr);

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

	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");
	Model sky((char*)"Models/sky.obj");

	//KeyFrames - Inicialización con las nuevas variables
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].HeadInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].FLegs = 0;      // Inicializar patas delanteras
		KeyFrame[i].FLegsInc = 0;   // Inicializar incremento patas delanteras
		KeyFrame[i].RLegs = 0;      // Inicializar patas traseras
		KeyFrame[i].RLegsInc = 0;   // Inicializar incremento patas traseras
		KeyFrame[i].tail = 0;       // Inicializar cola
		KeyFrame[i].tailInc = 0;    // Inicializar incremento cola
		KeyFrame[i].bodyHeight = 0; // Inicializar altura del cuerpo
		KeyFrame[i].bodyHeightInc = 0; // Inicializar incremento altura del cuerpo
	}

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
		Animation();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f);

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

		glm::mat4 model(1);

		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		//Body - ahora incluye bodyHeight en la posición Y
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY + bodyHeight, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);

		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);

		//Front Left Leg - AMBAS patas delanteras usan el MISMO ángulo FLegs
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);

		//Front Right Leg - AMBAS patas delanteras usan el MISMO ángulo FLegs
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f)); // MISMO sentido que izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);

		//Back Left Leg - AMBAS patas traseras usan el MISMO ángulo RLegs
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);

		//Back Right Leg - AMBAS patas traseras usan el MISMO ángulo RLegs
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f)); // MISMO sentido que izquierda
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);
		//Cielo
		model = modelTemp;
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)); // Escala uniforme 2x
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sky.Draw(lightingShader);

		model = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		// Also draw the lamp object
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4(1);
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
	//Dog Controls
	if (keys[GLFW_KEY_4])
	{
		head += 0.5f;
	}
	if (keys[GLFW_KEY_5])
	{
		head -= 0.5f;
	}
	if (keys[GLFW_KEY_2])
	{
		rotDog += 0.5f;
	}
	if (keys[GLFW_KEY_3])
	{
		rotDog -= 0.5f;
	}
	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.01f;
	}
	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.01f;
	}

	// Controles MODIFICADOS para patas en el MISMO sentido
	if (keys[GLFW_KEY_6])  // AMBAS patas delanteras hacia adelante
	{
		FLegs += 0.01f;
	}
	if (keys[GLFW_KEY_7])  // AMBAS patas delanteras hacia atrás
	{
		FLegs -= 0.01f;
	}
	if (keys[GLFW_KEY_8])  // AMBAS patas traseras hacia adelante
	{
		RLegs += 0.01f;
	}
	if (keys[GLFW_KEY_9])  // AMBAS patas traseras hacia atrás
	{
		RLegs -= 0.01f;
	}
	if (keys[GLFW_KEY_0])  // Cola hacia arriba
	{
		tail += 0.01f;
	}
	if (keys[GLFW_KEY_MINUS])  // Cola hacia abajo
	{
		tail -= 0.5f;
	}

	// NUEVOS CONTROLES para torso y cabeza juntos
	if (keys[GLFW_KEY_Q])  // Subir torso y bajar cabeza (para agacharse)
	{
		moveBodyAndHeadUp();
	}
	if (keys[GLFW_KEY_Z])  // Bajar torso y subir cabeza (para levantarse)
	{
		moveBodyAndHeadDown();
	}

	// Camera controls
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
	// Tecla para guardar animación en archivo - CORREGIDO
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		saveAnimationToFile();
	}

	// Tecla para cargar animación desde archivo - CORREGIDO  
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		loadAnimationFromFile();
	}

	// Tecla para reproducir animación
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
			std::cout << "Iniciando reproduccion de animacion..." << std::endl;
		}
		else
		{
			play = false;
			std::cout << "Animacion pausada" << std::endl;
		}
	}

	// Tecla para guardar frame
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
			std::cout << "Frame " << FrameIndex << " guardado" << std::endl;
		}
		else
		{
			std::cout << "Maximo de frames alcanzado: " << MAX_FRAMES << std::endl;
		}
	}

	// Tecla para sentar al perro - CORREGIDO
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		sitDog();
		std::cout << "Perro sentado" << std::endl;
	}

	// Tecla para parar al perro - CORREGIDO
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		standDog();
		std::cout << "Perro de pie" << std::endl;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			std::cout << "Luz especial activada" << std::endl;
		}
		else
		{
			Light1 = glm::vec3(0);
			std::cout << "Luz especial desactivada" << std::endl;
		}
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
}

void Animation() {
	if (play)
	{
		if (i_curr_steps >= i_max_steps)
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)
			{
				printf("Animacion terminada\n");
				playIndex = 0;
				play = false;
			}
			else
			{
				i_curr_steps = 0;
				interpolation();
			}
		}
		else
		{
			// Animación con todas las partes incluidas
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].HeadInc;
			rotDog += KeyFrame[playIndex].rotDogInc;
			FLegs += KeyFrame[playIndex].FLegsInc;  // Animación patas delanteras
			RLegs += KeyFrame[playIndex].RLegsInc;  // Animación patas traseras
			tail += KeyFrame[playIndex].tailInc;    // Animación cola
			bodyHeight += KeyFrame[playIndex].bodyHeightInc; // Animación altura cuerpo

			i_curr_steps++;
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