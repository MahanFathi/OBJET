#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "model.h"

class OBJET
{
public:
    /* Data */


    /* Function */
    OBJET(std::string pathToMetaJSON);
    void DrawToImage(std::string pathToImage);

protected:
    /* Data */
    GLFWwindow* window;
    Shader* objectShader;
    Model* model;

    /* Function */
    void InitOpenGL();
    void InitShader();
    void InitModel(std::string pathToMetaJSON);

};
