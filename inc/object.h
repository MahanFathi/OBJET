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
    /*  Model Data */
    std::vector<Mesh> meshes;
    std::string name;
    std::string directory;
    data::ObjectData objectData;

    /* Functions */
    Object(data::ObjectData &objectData, std::string &name);

    void draw(Shader shader);

private:
    /*  Functions   */
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};
