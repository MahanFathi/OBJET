#include "model.h"

Model::Model(data::MetaData &metaData):
    metaData(metaData)
{
    for (unsigned int i = 0; i < metaData.objectCount; i++) {
        std::string objectJSONPath = metaData.objectJSONPaths[i];
        std::string objectName = metaData.objectNames[i];
        data::ObjectData objectData(objectJSONPath.c_str());
        objects.push_back(Object(objectData, objectName));
    }
}

void Model::draw(Shader shader)
{
    setLightProperties(shader);
    for(unsigned int i = 0; i < objects.size(); i++) {
        setTransformations(shader, i);
        objects[i].draw(shader);
    }
}


void Model::setLightProperties(Shader shader)
{
    // directional lighting
    shader.setUniform("directionalLigth.direction", metaData.directionalLightDirection);
    shader.setUniform("directionalLigth.color", metaData.directionalLightColor);

    // point lighting
    shader.setUniform("pointLightCount", metaData.pointLightCount);
    for(unsigned int i = 0; i < metaData.pointLightCount; i++) {
        shader.setUniform("pointLights.[" + std::to_string(i) + "].position", metaData.pointLightPositions[i]);
        shader.setUniform("pointLights.[" + std::to_string(i) + "].color", metaData.pointLightColors[i]);
    }
}


void Model::setTransformations(Shader shader, unsigned int objectNum)
{
    // scale and rotate
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, metaData.objectYRotations[objectNum], glm::vec3(0.0f, 1.0f, 0.0f));
    rotation = glm::scale(rotation, metaData.objectScales[objectNum] * glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("transform", rotation);

    // translate
    glm::mat4 translate = glm::mat4(1.0f);
    translate = glm::translate(translate, metaData.objectTranslations[objectNum]);
    shader.setUniform("model", translate);

}
