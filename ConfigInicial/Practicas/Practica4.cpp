//Previo 4
//30 de agosto de 2025
//Palafox Jimenez Rául
//422132844
#include<iostream>
#include<vector>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);
std::vector<float> generateCubeVertices(glm::vec3 color);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico Palafox Jimenez Raul", nullptr, nullptr);
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Build and compile our shader program
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Definir colores (RGB normalizado)
    glm::vec3 color1(99.0f / 255.0f, 178.0f / 255.0f, 211.0f / 255.0f);  // #63B2D3
    glm::vec3 color2(254.0f / 255.0f, 255.0f / 255.0f, 235.0f / 255.0f); // #FEFFEB
    glm::vec3 color3(202.0f / 255.0f, 180.0f / 255.0f, 149.0f / 255.0f); // #CAB495
    glm::vec3 color4(0.0f, 0.0f, 0.0f);                            // Negro

    // Generar vértices para cada cubo
    std::vector<float> vertices1 = generateCubeVertices(color1);
    std::vector<float> vertices2 = generateCubeVertices(color2);
    std::vector<float> vertices3 = generateCubeVertices(color3);
    std::vector<float> vertices4 = generateCubeVertices(color4);

    // Crear 4 VAOs y VBOs
    GLuint VAOs[4], VBOs[4];
    glGenVertexArrays(4, VAOs);
    glGenBuffers(4, VBOs);

    // Configurar cada VAO
    for (int i = 0; i < 4; i++) {
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);

        // Usar los vértices correspondientes
        if (i == 0) glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(float), vertices1.data(), GL_STATIC_DRAW);
        if (i == 1) glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(float), vertices2.data(), GL_STATIC_DRAW);
        if (i == 2) glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(float), vertices3.data(), GL_STATIC_DRAW);
        if (i == 3) glBufferData(GL_ARRAY_BUFFER, vertices4.size() * sizeof(float), vertices4.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();

        // Render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw our cubes
        ourShader.Use();
        glm::mat4 model = glm::mat4(1);
        glm::mat4 view = glm::mat4(1);

        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // SOLO CUBO 1 VISIBLE (#63B2D3 - Azul claro)
        glBindVertexArray(VAOs[0]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  // Posición central
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));      // Escala normal
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*
        // CUBO 2 COMENTADO (#FEFFEB - Crema)
        glBindVertexArray(VAOs[1]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, 0.3f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */

        /*
        // CUBO 3 COMENTADO (#CAB495 - Beige)
        glBindVertexArray(VAOs[2]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 0.3f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */

        /*
        // CUBO 4 COMENTADO (Negro)
        glBindVertexArray(VAOs[3]);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(4, VAOs);
    glDeleteBuffers(4, VBOs);
    glfwTerminate();
    return EXIT_SUCCESS;
}

std::vector<float> generateCubeVertices(glm::vec3 color) {
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,

        -0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
        -0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,

         0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,

        -0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,

        -0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f, -0.5f,-0.5f,   color.r, color.g, color.b,

        -0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f,-0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
         0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f,  0.5f, 0.5f,   color.r, color.g, color.b,
        -0.5f, 0.5f,-0.5f,   color.r, color.g, color.b
    };
    return vertices;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.008f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.008f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        movY += 0.008f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        movY -= 0.008f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.008f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.008f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.04f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.04f;
}
