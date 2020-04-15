#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "data.h"
#include "mesh.h"
#include "shader.h"

class Object
{
public:
    /*  Data */
    const std::string name;
    std::vector<Mesh> meshes;
    data::ObjectData* objectData;

    /* Functions */
    Object(const char* pathToObjectJSON, const std::string &name);

    void draw(Shader* shader) const;

private:
    /*  Functions   */
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};
