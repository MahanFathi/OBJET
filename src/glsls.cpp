//  create a shader self-contained shared object
#include <string>

std::string objectVertexShader =
#include "shaders/vertex_shader.glsl"
    ;

std::string objectFragmentShader =
#include "shaders/fragment_shader.glsl"
    ;

std::string shadowVertexShader =
#include "shaders/shadow_vertex_shader.glsl"
    ;

std::string shadowFragmentShader =
#include "shaders/shadow_fragment_shader.glsl"
    ;
