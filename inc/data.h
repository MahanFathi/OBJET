#pragma once

#include <string>
#include <vector>
#include <rapidjson/document.h>
#include <glm/glm.hpp>


rapidjson::Document readJSON(const char *pathToJSON);

namespace data {

class MetaData
{
public:
    /* Data */
    // objects
    int objectCount;
    std::vector<std::string> objectNames;
    std::vector<std::string> objectJSONPaths;
    std::vector<glm::vec3> objectTranslations;
    std::vector<float> objectYRotations;
    std::vector<float> objectScales;
    // camera
    glm::vec3 cameraPosition;
    glm::vec3 cameraTarget;
    // lighting
    int pointLightCount;
    std::vector<glm::vec3> pointLightPositions;
    std::vector<glm::vec3> pointLightColors;
    glm::vec3 directionalLightDirection;
    glm::vec3 directionalLightColor;

    /* Functions */
    MetaData(const char *pathToObjectJSON);

};

class ObjectData
{
public:
    /* Data */
    std::string objPath;
    glm::vec3 color;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;

    /* Functions */
    ObjectData(const char *pathToObjectJSON);
};

}
