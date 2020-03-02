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
    for(unsigned int i = 0; i < objects.size(); i++) {
       
        // scale and rotate
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation = glm::rotate(rotation, metaData.objectYRotations[i], glm::vec3(0.0f, 1.0f, 0.0f));
        rotation = glm::scale(rotation, metaData.objectScales[i] * glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setUniform("transform", rotation);

        // translate
        glm::mat4 translate = glm::mat4(1.0f);
        translate = glm::translate(translate, metaData.objectTranslations[i]);
        shader.setUniform("model", translate);

        objects[i].draw(shader);
    }
}
