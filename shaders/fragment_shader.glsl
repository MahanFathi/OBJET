#version 430 core

struct Material {
    vec3 color;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 color;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 FragColor;


#define NR_POINT_LIGHTS 4

uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLigth;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform int pointLightCount;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    vec3 viewDirection = normalize(cameraPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, fragNormal);

    // diffusion
    float diffusion = material.diffuseStrength *
        max(dot(fragNormal, lightDirection), 0.0);

    // specular
    float specular = material.specularStrength *
        pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    // directional light
    vec3 result = CalcDirLight(directionalLigth, fragNormal, viewDirection);

    // point lights
    for(int i = 0; i < max(NR_POINT_LIGHTS, pointLightCount); i++)
        result += CalcPointLight(pointLights[i], fragNormal, fragPosition, viewDirection);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    float ambient  = material.ambientStrength;
    float diffuse  = material.diffuseStrength  * diff;
    float specular = material.specularStrength * spec;
    return ((ambient + diffuse + specular) * light.color * material.color);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    float ambient  = material.ambientStrength;
    float diffuse  = material.diffuseStrength  * diff;
    float specular = material.specularStrength * spec;
    return ((ambient + diffuse + specular) * light.color * material.color);
}
