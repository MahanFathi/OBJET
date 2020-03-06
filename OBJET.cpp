#include "OBJET.h"

#include <iterator>

#include <FreeImage.h>

#include "util.h"

OBJET::OBJET(std::string pathToMetaJSON)
{
    InitOpenGL();
    InitShader();
    InitModel(pathToMetaJSON);
}


void OBJET::Draw()
{
    // use shader
    objectShader->use();

    // use specific buffer to draw
    // glDrawBuffer(GL_FRONT);

    glClearColor(0.55f, 0.77, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // draw object
    model->setEnvironmentPorperties(objectShader);
    model->draw(objectShader);

    glFlush();
}


std::vector<int> OBJET::GetImage()
{
    // Make the BYTE array
    int width = 500;
    int height = 500;
    GLubyte* pixels = new GLubyte[3 * width * height];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    std::vector<int> image;
    for (int i = 0; i < 3 * width * height; i++)
        image.push_back(static_cast<int>(pixels[i]));

    return image;
}


void OBJET::ToImage(std::string pathToImage)
{
    // Make the BYTE array
    int width = 500;
    int height = 500;
    GLubyte* pixels = new GLubyte[3 * width * height];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
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

    // off-screen rendering
    // weird stuff happens in i3 for example when rendering to
    // the main buffer, the desktop manager messes with window
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // renderbuffer object
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 500, 500);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
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
