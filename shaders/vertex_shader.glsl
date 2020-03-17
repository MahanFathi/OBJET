R"glsl(

#version 430 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 fragNormal;
out vec3 fragPosition;
out vec4 fragPosLightSpace;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * transform * vec4(vertexPosition, 1.0);
    fragPosition = vec3(model * transform * vec4(vertexPosition, 1.0));
    fragNormal = normalize(vec3(transform * vec4(vertexNormal, 1.0)));
    fragPosLightSpace = lightSpaceMatrix * model * transform * vec4(vertexPosition, 1.0);
}

)glsl"
