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
    data::MetaData metaData;
    std::vector<Object> objects; // model contains a bunch of objects, e.g. the room, or objects

    /*  Functions   */
    Model(data::MetaData &metaData);
   
    void draw(Shader shader);
};
