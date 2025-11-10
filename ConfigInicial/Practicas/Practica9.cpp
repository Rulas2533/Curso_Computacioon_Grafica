// Palafox Jimenez Raúl
// Practica 9
// No. De Cuenta: 422132844
// 19 de Octubre de 2025
#include <iostream>
#include <cmath>

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

// Load Models
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
bool sunActive = true; // Control para la luz de vitrina
bool lampLightsActive = true; // Control para las luces de las lámparas
glm::vec3 lampLightColor = glm::vec3(1.2f, 1.1f, 0.9f); // Color blanco menos brillante

// Positions of the point lights and spotlights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    // Luz de vitrina (point light) - APAGADA
    glm::vec3(0.0f, 0.0f, 0.0f),    // Luz de vitrina secundaria - APAGADA
    glm::vec3(0.0f, 1.8f, -1.0f),   // Luz de lámpara 1 (POSICIÓN ACTUALIZADA EN TIEMPO REAL)
    glm::vec3(0.0f, 1.8f, 3.0f)     // Luz de lámpara 2 (POSICIÓN ACTUALIZADA EN TIEMPO REAL)
};

// Posiciones de los spotlights (lámparas de museo)
glm::vec3 spotlightPositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),   // Spotlight 1 (lámpara izquierda) - APAGADO
    glm::vec3(2.0f, 3.0f, 0.0f),    // Spotlight 2 (lámpara derecha) - APAGADO
    glm::vec3(0.0f, 2.5f, -2.7f)    // LUZ ESTATUA - Spotlight 3 (nueva luz para la estatua)
};

// Direcciones de los spotlights (apuntando hacia abajo)
glm::vec3 spotlightDirections[] = {
    glm::vec3(0.0f, -1.0f, 0.0f),   // Dirección spotlight 1
    glm::vec3(0.0f, -1.0f, 0.0f),   // Dirección spotlight 2
    glm::vec3(0.0f, -1.0f, 0.0f)    // Dirección LUZ ESTATUA - apuntando hacia abajo
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
glm::vec3 VitrinaLight = glm::vec3(0.8f, 0.9f, 1.0f); // Color azul claro para vitrina

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;    // Time of last frame

int main()
{
    // Init GLFW
    glfwInit();

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz Palafox Jimenez Raul - Museo con Spotlights", nullptr, nullptr);

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

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model Dog((char*)"Models/sala.obj");
    Model Esc((char*)"Models/estatua.obj");
    Model Lamp((char*)"Models/Lamp.obj");
    Model Lamp2((char*)"Models/lP2.obj");


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
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL options
        glEnable(GL_DEPTH_TEST);

        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light - APAGADA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);

        // Point light 0 (Luz de vitrina) - APAGADA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0f);

        // Point light 1 (Luz de vitrina secundaria) - APAGADA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

        // Point light 2 (Lámpara 1 - MENOS BRILLANTE)
        if (lampLightsActive) {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lampLightColor.x * 0.1f, lampLightColor.y * 0.1f, lampLightColor.z * 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lampLightColor.x * 0.8f, lampLightColor.y * 0.8f, lampLightColor.z * 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), lampLightColor.x * 0.9f, lampLightColor.y * 0.9f, lampLightColor.z * 0.9f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f); // Mayor atenuación
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);
        }

        // Point light 3 (Lámpara 2 - MENOS BRILLANTE)
        if (lampLightsActive) {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lampLightColor.x * 0.1f, lampLightColor.y * 0.1f, lampLightColor.z * 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lampLightColor.x * 0.8f, lampLightColor.y * 0.8f, lampLightColor.z * 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), lampLightColor.x * 0.9f, lampLightColor.y * 0.9f, lampLightColor.z * 0.9f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f); // Mayor atenuación
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);
        }

        // SpotLight 1 (Lámpara de museo izquierda) - APAGADA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"), spotlightPositions[0].x, spotlightPositions[0].y, spotlightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"), spotlightDirections[0].x, spotlightDirections[0].y, spotlightDirections[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(17.5f)));

        // SpotLight 2 (Lámpara de museo derecha) - APAGADA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].position"), spotlightPositions[1].x, spotlightPositions[1].y, spotlightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"), spotlightDirections[1].x, spotlightDirections[1].y, spotlightDirections[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(17.5f)));

        // ========== LUZ ESTATUA - Spotlight 3 (nueva luz para la estatua) ==========
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].position"), spotlightPositions[2].x, spotlightPositions[2].y, spotlightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].direction"), spotlightDirections[2].x, spotlightDirections[2].y, spotlightDirections[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].ambient"), 0.05f, 0.05f, 0.05f); // Muy tenue
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].diffuse"), 0.4f, 0.4f, 0.35f);   // Luz suave
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].specular"), 0.3f, 0.3f, 0.25f);  // Specular bajo
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].linear"), 0.14f);      // Mayor atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].quadratic"), 0.07f);   // Mayor atenuación
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].cutOff"), glm::cos(glm::radians(15.0f)));     // Ángulo más amplio
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].outerCutOff"), glm::cos(glm::radians(25.0f))); // Ángulo más amplio
        // ========== FIN LUZ ESTATUA ==========

        // SpotLight de cámara - APAGADA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Set material properties - MÁRMOL MODERADO
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f); // Más bajo para menos brillo

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //modelo museo
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Trasladar
        model = glm::scale(model, glm::vec3(0.25f)); // Escalar
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Dog.Draw(lightingShader);
        //Estatua
        glm::mat4 model2(1);
        model2 = glm::translate(model2, glm::vec3(0.0f, -2.0f, -2.7f)); // Trasladar a posición diferente
        model2 = glm::scale(model2, glm::vec3(1.5f)); // Escalar diferente
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        Esc.Draw(lightingShader);

        //Lampara 1
        glm::mat4 model3(1);
        model3 = glm::translate(model3, glm::vec3(0.0f, 1.8f, -1.0f)); // Trasladar
        model3 = glm::rotate(model3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar 90° en Y
        model3 = glm::scale(model3, glm::vec3(0.1f)); // Escalar
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        Lamp.Draw(lightingShader);

        //Lampara 2
        glm::mat4 model4(1);
        model4 = glm::translate(model4, glm::vec3(0.0f, 1.8f, 3.0f)); // Trasladar a posición diferente
        model4 = glm::rotate(model4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar 90° en Y
        model4 = glm::scale(model4, glm::vec3(0.1f)); // Escalar
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
        Lamp.Draw(lightingShader); // Asumiendo que usas el mismo modelo para ambas lámparas
        //Lampara de objeto por encima:
        glm::mat4 model5(1);
        model5 = glm::translate(model5, glm::vec3(0.0f, 2.0f, -2.7f)); // Trasladar a posición diferente
        model5 = glm::rotate(model5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotar 90° en Y
        model5 = glm::scale(model5, glm::vec3(0.25f)); // Escalar
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
        Lamp2.Draw(lightingShader); // Asumiendo que usas el mismo modelo para ambas lámparas


        // ACTUALIZAR POSICIONES DE LUCES DE LÁMPARAS para que sigan a los modelos
        pointLightPositions[2] = glm::vec3(0.0f, 1.7f, -1.0f); // Seguir lámpara 1
        pointLightPositions[3] = glm::vec3(0.0f, 1.7f, 3.0f);  // Seguir lámpara 2

        // ACTUALIZAR POSICIÓN LUZ ESTATUA para que siga a la estatua
        spotlightPositions[2] = glm::vec3(0.0f, 2.0f, -2.7f); // 2.5 unidades por encima de la estatua

        glBindVertexArray(0);


        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the point light objects
        for (GLuint i = 0; i < 4; i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);

            // Luces de lámparas menos brillantes visualmente
            if ((i == 2 || i == 3) && lampLightsActive) {
                model = glm::scale(model, glm::vec3(0.15f)); // Más pequeñas
                glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"),
                    lampLightColor.x * 0.7f, lampLightColor.y * 0.7f, lampLightColor.z * 0.7f);
            }
            else {
                model = glm::scale(model, glm::vec3(0.1f));
                glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.1f, 0.1f, 0.1f);
            }

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw the spotlight objects (representaciones visuales)
        for (GLuint i = 0; i < 3; i++) // Ahora son 3 spotlights
        {
            model = glm::mat4(1);
            model = glm::translate(model, spotlightPositions[i]);
            model = glm::scale(model, glm::vec3(0.15f));

            if (i == 2) {
                // LUZ ESTATUA - color suave
                glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.6f, 0.6f, 0.5f);
            }
            else {
                glUniform3f(glGetUniformLocation(lampShader.Program, "lampColor"), 0.1f, 0.1f, 0.1f); // Color oscuro para spotlights apagados
            }

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
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

    // Controles para la Luz de vitrina principal (AHORA APAGADA)
    if (keys[GLFW_KEY_T])
    {
        pointLightPositions[0].x += 0.01f;
    }
    if (keys[GLFW_KEY_G])
    {
        pointLightPositions[0].x -= 0.01f;
    }
    if (keys[GLFW_KEY_Y])
    {
        pointLightPositions[0].y += 0.01f;
    }
    if (keys[GLFW_KEY_H])
    {
        pointLightPositions[0].y -= 0.01f;
    }
    if (keys[GLFW_KEY_U])
    {
        pointLightPositions[0].z -= 0.01f;
    }
    if (keys[GLFW_KEY_J])
    {
        pointLightPositions[0].z += 0.01f;
    }

    // Controles para la Luz de vitrina secundaria (AHORA APAGADA)
    if (keys[GLFW_KEY_I])
    {
        pointLightPositions[1].x += 0.01f;
    }
    if (keys[GLFW_KEY_K])
    {
        pointLightPositions[1].x -= 0.01f;
    }
    if (keys[GLFW_KEY_O])
    {
        pointLightPositions[1].y += 0.01f;
    }
    if (keys[GLFW_KEY_L])
    {
        pointLightPositions[1].y -= 0.01f;
    }
    if (keys[GLFW_KEY_P])
    {
        pointLightPositions[1].z -= 0.01f;
    }
    if (keys[GLFW_KEY_SEMICOLON])
    {
        pointLightPositions[1].z += 0.01f;
    }

    // NUEVOS CONTROLES para los Spotlights (solo movimiento vertical - teclas F1 a F4) - AHORA APAGADOS
    if (keys[GLFW_KEY_F1])
    {
        spotlightPositions[0].y -= 0.01f; // Spotlight 1 hacia abajo
    }
    if (keys[GLFW_KEY_F2])
    {
        spotlightPositions[0].y += 0.01f; // Spotlight 1 hacia arriba
    }
    if (keys[GLFW_KEY_F3])
    {
        spotlightPositions[1].y -= 0.01f; // Spotlight 2 hacia abajo
    }
    if (keys[GLFW_KEY_F4])
    {
        spotlightPositions[1].y += 0.01f; // Spotlight 2 hacia arriba
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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

    // Control para activar/desactivar las luces de las lámparas
    if (keys[GLFW_KEY_L] && action == GLFW_PRESS) {
        lampLightsActive = !lampLightsActive;
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