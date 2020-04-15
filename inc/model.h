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
    void setCamera(const std::vector<float> &position, const std::vector<float> &target);
    void setObjectPosition(const std::string &objectName, const std::vector<float> &position);
    void setObjectYRotation(const std::string &objectName, const float &yRotation);
    void setObjectScale(const std::string &objectName, const float &scale);

private:
    /* Functions */
    void setTransformations(Shader* shader, const unsigned int &objectNum);
    void setLightProperties(Shader* shader);
    void setCamera(Shader* shader);
    unsigned objectNameToIndex(const std::string &objectName) const;
};
