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
in vec4 fragPosLightSpace;

out vec4 FragColor;


#define NR_POINT_LIGHTS 4

uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform sampler2D shadowMap;
uniform vec3 cameraPosition;
uniform int pointLightCount;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shadow);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);


void main()
{
    vec3 viewDirection = normalize(cameraPosition - fragPosition);

    // directional light
    float shadow = ShadowCalculation(fragPosLightSpace, fragNormal, directionalLight.direction);
    vec3 result = CalcDirLight(directionalLight, fragNormal, viewDirection, shadow);

    // point lights
    for(int i = 0; i < min(NR_POINT_LIGHTS, pointLightCount); i++)
        result += CalcPointLight(pointLights[i], fragNormal, fragPosition, viewDirection);

    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, float shadow)
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
    return ((ambient + (diffuse + specular) * (1.0 - shadow)) * light.color * material.color);
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


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
