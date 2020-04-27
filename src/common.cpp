#include <glm/glm.hpp>
#include "common.h"


// cam settings
float deltaTime = 0.0f;
glm::vec3 cameraPos   = glm::vec3(7.0f, 4.0f,  7.0f);
glm::vec3 cameraFront = glm::normalize(-cameraPos);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
