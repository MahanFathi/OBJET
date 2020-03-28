#include "model.h"

Model::Model(const char* pathToMetaJSON)
{
    metaData = new data::MetaData(pathToMetaJSON);
    for (unsigned int i = 0; i < metaData->objectCount; i++) {
        std::string pathToObjectJSON = metaData->objectJSONPaths[i];
        std::string objectName = metaData->objectNames[i];
        objects.push_back(Object(pathToObjectJSON.c_str(), objectName));
    }
}


void Model::setEnvironment(Shader* shader)
{
    setShadow(shader);
    setCamera(shader);
    setLightProperties(shader);
}


void Model::draw(Shader* shader)
{
    for(unsigned int i = 0; i < objects.size(); i++) {
        setTransformations(shader, i);
        objects[i].draw(shader);
    }
}


void Model::setLightProperties(Shader* shader)
{
    // directional lighting
    shader->setUniform("directionalLight.direction", glm::normalize(metaData->directionalLightDirection));
    shader->setUniform("directionalLight.color", metaData->directionalLightColor);

    // point lighting
    shader->setUniform("pointLightCount", metaData->pointLightCount);
    for(unsigned int i = 0; i < metaData->pointLightCount; i++) {
        shader->setUniform("pointLights[" + std::to_string(i) + "].position", metaData->pointLightPositions[i]);
        shader->setUniform("pointLights[" + std::to_string(i) + "].color", metaData->pointLightColors[i]);
    }
}


void Model::setTransformations(Shader* shader, const unsigned int &objectNum)
{
    // scale and rotate
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, metaData->objectYRotations[objectNum], glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::scale(rotation, metaData->objectScales[objectNum] * glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("transform", rotation);

    // translate
    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, metaData->objectTranslations[objectNum]);
    shader->setUniform("model", translate);

}


void Model::setCamera(Shader* shader)
{
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0);
    glm::mat4 view = glm::lookAt(
        metaData->cameraPosition,
        -metaData->cameraPosition + metaData->cameraTarget,
        cameraUp);
    shader->setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(metaData->cameraFieldOfView), 1.0f, // FIXME: aspect ratio
                                  metaData->cameraPerspectiveNearPlane,
                                  metaData->cameraPerspectiveFarPlane);
    shader->setUniform("projection", projection);
}


void Model::setShadow(Shader* shader)
{
    float scale = 10;
    float bound = 30.0f, near_plane = 5.0f, far_plane = 20.0f;
    glm::mat4 lightProjection = glm::ortho(-bound, bound, -bound, bound, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(-scale * glm::normalize(metaData->directionalLightDirection),
                                      glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    shader->setUniform("lightSpaceMatrix", lightSpaceMatrix);
}


unsigned Model::objectNameToIndex(const std::string &objectName)
{
    std::ptrdiff_t pos = distance(
        metaData->objectNames.begin(),
        find(metaData->objectNames.begin(),
             metaData->objectNames.end(),
             objectName)
    );
    return unsigned(pos);
}


void Model::setCamera(const std::vector<float> &position, const std::vector<float> &target)
{
    metaData->cameraPosition = glm::vec3(
        position[0],
        position[1],
        position[2]
    );
    metaData->cameraTarget = glm::vec3(
        target[0],
        target[1],
        target[2]
    );
}


void Model::setObjectPosition(const std::string &objectName, const std::vector<float> &position)
{
    unsigned index = objectNameToIndex(objectName);
    metaData->objectTranslations[index] = glm::vec3(
        position[0],
        position[1],
        position[2]
    );
}


void Model::setObjectYRotation(const std::string &objectName, const float &yRotation)
{
    unsigned index = objectNameToIndex(objectName);
    metaData->objectYRotations[index] = yRotation;
}


void Model::setObjectScale(const std::string &objectName, const float &scale)
{
    unsigned index = objectNameToIndex(objectName);
    metaData->objectScales[index] = scale;
}
