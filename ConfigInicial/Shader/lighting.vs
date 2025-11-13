#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords; // Asumiendo que tu Model.h lo carga en la locación 2

// Salidas hacia el Fragment Shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calcula la posición en el espacio del mundo
    FragPos = vec3(model * vec4(aPos, 1.0)); 
    
    // Calcula la Normal en el espacio del mundo (usa la matriz normal para evitar deformaciones)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pasa las coordenadas de textura
    TexCoords = aTexCoords;
    
    // Calcula la posición final en la pantalla
    gl_Position = projection * view * vec4(FragPos, 1.0);
}