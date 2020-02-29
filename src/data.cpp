#include <iostream>
#include <fstream>

#include "data.h"


data::MetaData::MetaData(const char *pathToMetaJSON) {
    rapidjson::Document metaJSON = readJSON(pathToMetaJSON);
    rapidjson::Value& objects = metaJSON["objects"];

    objectCount = objects.Size();

    /* parse the data into object */
    for (rapidjson::SizeType i = 0; i < objects.Size(); i++) {
        // get strings
        objectNames.push_back(objects[i]["name"].GetString());
        objectJSONPaths.push_back(objects[i]["path_to_json"].GetString());
        //get rotations and translations
        float objectYRotation = objects[i]["rotation_y"].GetFloat();
        objectYRotations.push_back(objectYRotation);
        float translation_x = objects[i]["translation"]["x"].GetFloat();
        float translation_y = objects[i]["translation"]["y"].GetFloat();
        float translation_z = objects[i]["translation"]["z"].GetFloat();
        objectTranslations.push_back(
            glm::vec3(translation_x, translation_y, translation_z)
        );
    }
}


data::ObjectData::ObjectData(const char *pathToObjectJSON) {
    rapidjson::Document objJSON = readJSON(pathToObjectJSON);

    objPath = objJSON["path_to_obj"].GetString();
    color = glm::vec3(
        float(objJSON["color"]["r"].GetInt()) / 255.0f,
        float(objJSON["color"]["g"].GetInt()) / 255.0f,
        float(objJSON["color"]["b"].GetInt()) / 255.0f
    );
    scale = objJSON["scale"].GetFloat();
    ambientStrength = objJSON["ambient_strength"].GetFloat();
    diffuseStrength = objJSON["diffuse_strength"].GetFloat();
    specularStrength = objJSON["specular_strength"].GetFloat();
}


rapidjson::Document readJSON(const char *pathToJSON) {
    std::ifstream jsonFile(pathToJSON);
    std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

    rapidjson::Document jsonDoc;
    jsonDoc.Parse(json.c_str());

    return jsonDoc;
}
