#version 330 core
out vec4 FragColor;

// --- Estructuras de Datos ---
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

// --- Variables de Entrada/Uniformes ---
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform Material material;

// --- ¡¡AQUÍ ESTÁ EL CAMBIO IMPORTANTE!! ---
// Definimos el número de luces que C++ nos va a enviar
#define NR_POINT_LIGHTS 17 
// Definimos 'pointLights' como un ARRAY de 17
uniform PointLight pointLights[NR_POINT_LIGHTS]; 

// --- Funciones de Cálculo de Luz ---

// Calcula la luz direccional (Sol)
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
}

// Calcula UNA luz de punto (Lámpara)
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    // Atenuación
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

// Calcula la luz de la linterna (Cámara)
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if(intensity > 0.0) // Si no está dentro del cono, no calcules
    {
        // Ambient, Diffuse, Specular
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
        // Atenuación
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        
        ambient *= attenuation * intensity;
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;
        
        return (ambient + diffuse + specular);
    }
    else
        return vec3(0.0, 0.0, 0.0);
}


// --- ¡¡FUNCIÓN main() CON LOOP DESENREDADO!! ---
void main()
{
    // Propiedades comunes
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // 1. Luz Direccional (Sol)
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // 2. ¡¡LOOP DE POINT LIGHTS DESENREDADO MANUALMENTE!!
    result += CalcPointLight(pointLights[0], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[1], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[2], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[3], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[4], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[5], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[6], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[7], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[8], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[9], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[10], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[11], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[12], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[13], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[14], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[15], norm, FragPos, viewDir);
    result += CalcPointLight(pointLights[16], norm, FragPos, viewDir);
    // --- FIN DEL LOOP DESENREDADO ---
        
    // 3. Spot Light (Linterna)
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}