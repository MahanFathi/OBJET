R"glsl(

#version 430 core

layout (location = 0) in vec3 vertexPosition;

uniform mat4 lightSpaceMatrix;
uniform mat4 transform;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * transform * vec4(vertexPosition, 1.0);
} 

)glsl"
