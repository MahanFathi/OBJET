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
    // camera
    glm::vec3 cameraPosition;
    glm::vec3 cameraLookAt;
    // lighting
    glm::vec3 environmentColor;
    std::vector<glm::vec3> lightingPosition;
    std::vector<glm::vec3> lightingColor;

    /* Functions */
    MetaData(const char *pathToObjectJSON);

};

class ObjectData
{
public:
    /* Data */
    std::string objPath;
    glm::vec3 color;
    float scale;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;

    /* Functions */
    ObjectData(const char *pathToObjectJSON);
};

}
