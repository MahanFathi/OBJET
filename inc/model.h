#pragma once

#include <string>
#include <vector>

#include "data.h"
#include "object.h"
#include "shader.h"


class Model
{
public:
    /* Data */
    data::MetaData* metaData;
    std::vector<Object> objects; // model contains a bunch of objects, e.g. the room, or objects

    /*  Functions   */
    Model(const char *pathToObjectJSON);
    void setShadow(Shader* shader);
    void setEnvironment(Shader* shader);
    void draw(Shader* shader);
    void setCamera(std::vector<float> position, std::vector<float> target);
    void setObjectPosition(std::string &objectName, std::vector<float> position);
    void setObjectYRotation(std::string &objectName, float yRotation);
    void setObjectScale(std::string &objectName, float scale);

private:
    /* Functions */
    void setTransformations(Shader* shader, unsigned int objectNum);
    void setLightProperties(Shader* shader);
    void setCamera(Shader* shader);
    unsigned objectNameToIndex(std::string &objectName);
};
