#include <iostream>
#include <fstream>

#include "data.h"


data::MetaData::MetaData(const char *pathToMetaJSON) {
    rapidjson::Document metaJSON = readJSON(pathToMetaJSON);

    rapidjson::Value& objects = metaJSON["objects"];
    objectCount = objects.Size();

    /* parse the data into object */
    for (rapidjson::SizeType i = 0; i < objectCount; i++) {
        // get strings
        objectNames.push_back(objects[i]["name"].GetString());
        objectJSONPaths.push_back(objects[i]["path_to_json"].GetString());
        //get scales, rotations, and translations
        float objectScale = objects[i]["scale"].GetFloat();
        objectScales.push_back(objectScale);
        float objectYRotation = objects[i]["rotation_y"].GetFloat();
        objectYRotations.push_back(objectYRotation);
        float translation_x = objects[i]["translation"]["x"].GetFloat();
        float translation_y = objects[i]["translation"]["y"].GetFloat();
        float translation_z = objects[i]["translation"]["z"].GetFloat();
        objectTranslations.push_back(
            glm::vec3(translation_x, translation_y, translation_z)
        );
    }

    rapidjson::Value& pointLights = metaJSON["lighting"]["point_lights"];
    pointLightCount = pointLights.Size();

    /* parse the data into pointLights */
    for (rapidjson::SizeType i = 0; i < pointLightCount; i++) {
        // get position
        float position_x = pointLights[i]["position"]["x"].GetFloat();
        float position_y = pointLights[i]["position"]["y"].GetFloat();
        float position_z = pointLights[i]["position"]["z"].GetFloat();
        pointLightPositions.push_back(
            glm::vec3(position_x, position_y, position_z)
        );
        // get color
        float color_r = float(pointLights[i]["color"]["r"].GetInt()) / 255.0f;
        float color_g = float(pointLights[i]["color"]["g"].GetInt()) / 255.0f;
        float color_b = float(pointLights[i]["color"]["b"].GetInt()) / 255.0f;
        pointLightColors.push_back(
            glm::vec3(color_r, color_g, color_b)
        );
    }

    /* parse the data into directionalLigth */
    rapidjson::Value& directionalLight = metaJSON["lighting"]["directional_light"];
    float direction_x = directionalLight["direction"]["x"].GetFloat();
    float direction_y = directionalLight["direction"]["y"].GetFloat();
    float direction_z = directionalLight["direction"]["z"].GetFloat();
    directionalLightDirection = glm::vec3(
        direction_x, direction_y, direction_z
    );
    float color_r = float(directionalLight["color"]["r"].GetInt()) / 255.0f;
    float color_g = float(directionalLight["color"]["g"].GetInt()) / 255.0f;
    float color_b = float(directionalLight["color"]["b"].GetInt()) / 255.0f;
    directionalLightColor = glm::vec3(
        color_r, color_g, color_b
    );

    /* parse the data into camera */
    rapidjson::Value& camera = metaJSON["camera"];
    float position_x = camera["position"]["x"].GetFloat();
    float position_y = camera["position"]["y"].GetFloat();
    float position_z = camera["position"]["z"].GetFloat();
    cameraPosition = glm::vec3(
        position_x, position_y, position_z
    );
    float target_x = camera["target"]["x"].GetFloat();
    float target_y = camera["target"]["y"].GetFloat();
    float target_z = camera["target"]["z"].GetFloat();
    cameraTarget = glm::vec3(
        target_x, target_y, target_z
    );
    cameraFieldOfView = camera["perspective"]["field_of_view"].GetFloat();
    cameraPerspectiveNearPlane = camera["perspective"]["near_plane"].GetFloat();
    cameraPerspectiveFarPlane = camera["perspective"]["far_plane"].GetFloat();
}


data::ObjectData::ObjectData(const char *pathToObjectJSON) {
    rapidjson::Document objJSON = readJSON(pathToObjectJSON);

    objPath = objJSON["path_to_obj"].GetString();
    color = glm::vec3(
        float(objJSON["color"]["r"].GetInt()) / 255.0f,
        float(objJSON["color"]["g"].GetInt()) / 255.0f,
        float(objJSON["color"]["b"].GetInt()) / 255.0f
    );
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
