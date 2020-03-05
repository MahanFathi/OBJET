#include "OBJET.h"

#include <FreeImage.h>

#include "util.h"

OBJET::OBJET(std::string pathToMetaJSON)
{
    InitOpenGL();
    InitShader();
    InitModel(pathToMetaJSON);
}


void OBJET::DrawToImage(std::string pathToImage)
{

    // use shader
    objectShader->use();

    // use specific buffer to draw
    glDrawBuffer(GL_FRONT);

    glClearColor(0.55f, 0.77, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // draw object
    model->setEnvironmentPorperties(objectShader);
    model->draw(objectShader);

    glFlush();

    // Make the BYTE array
    int width = 500;
    int height = 500;
    GLubyte* pixels = new GLubyte[3 * width * height];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
    FreeImage_Save(FIF_PNG, image, pathToImage.c_str(), 0);
    FreeImage_Unload(image);
    delete [] pixels;
}


void OBJET::setObjectPosition(std::string objectName, std::vector<float> position)
{
    model->setObjectPosition(objectName, position);
}


void OBJET::setObjectYRotation(std::string objectName, float yRotation)
{
    model->setObjectYRotation(objectName, yRotation);
}


void OBJET::setObjectScale(std::string objectName, float scale)
{
    model->setObjectScale(objectName, scale);
}


void OBJET::InitOpenGL()
{
    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    window = glfwCreateWindow(500, 500, "OBJET", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwHideWindow(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
      exit(1);

    // hide and caputure the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // rendering window size
    glViewport(0, 0, 500, 500);

    // assign callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // run depth check (Z-buffer)
    glEnable(GL_DEPTH_TEST);
}


void OBJET::InitShader()
{
    objectShader = new Shader("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");
}


void OBJET::InitModel(std::string pathToMetaJSON)
{
    data::MetaData metaData = data::MetaData(pathToMetaJSON.c_str());
    model = new Model(metaData);
}
