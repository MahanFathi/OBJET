#pragma once

#include <string>
#include <glm/glm.hpp>

extern float deltaTime;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;

// create a shader self-contained shared object

static std::string objectVertexShader =
#include "shaders/vertex_shader.glsl"
    ;

static std::string objectFragmentShader =
#include "shaders/fragment_shader.glsl"
    ;

static std::string shadowVertexShader =
#include "shaders/shadow_vertex_shader.glsl"
    ;

static std::string shadowFragmentShader =
#include "shaders/shadow_fragment_shader.glsl"
    ;
