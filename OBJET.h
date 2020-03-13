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
    OBJET(std::string pathToMetaJSON, int width, int heigth);
    void Draw();
    void ToImage(std::string pathToImage);
    std::vector<float> GetDepthMap();
    std::vector<int> GetImage();
    void SetCamera(std::vector<float> position, std::vector<float> target);
    void SetObjectPosition(std::string objectName, std::vector<float> position);
    void SetObjectYRotation(std::string objectName, float yRotation);
    void SetObjectScale(std::string objectName, float scale);

protected:
    /* Data */
    GLuint renderFramebuffer;
    GLuint depthMapFramebuffer;
    GLuint depthMap;

    unsigned width, height;
    unsigned shadowWidth, shadowHeight;
    GLFWwindow* window;
    Shader* objectShader;
    Shader* shadowShader;
    Model* model;

    /* Function */
    void InitOpenGL();
    void InitShaders();
    void InitModel(std::string pathToMetaJSON);

};
