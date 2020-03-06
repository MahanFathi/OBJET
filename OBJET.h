#include <string>
#include <vector>

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
    void Draw();
    void ToImage(std::string pathToImage);
    std::vector<int> GetImage();
    void setObjectPosition(std::string objectName, std::vector<float> position);
    void setObjectYRotation(std::string objectName, float yRotation);
    void setObjectScale(std::string objectName, float scale);

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
