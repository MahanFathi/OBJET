#include "OBJET.h"

#include <FreeImage.h>

#include "glsls.h"
#include "util.h"

OBJET::OBJET(std::string pathToMetaJSON, int width, int height):
    width(width), height(height), shadowWidth(3 * width), shadowHeight(3 * height)
{
    InitOpenGL();
    InitShaders();
    InitModel(pathToMetaJSON);
}


void OBJET::Draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);

    shadowShader->use();
    model->setShadow(shadowShader);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFramebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    model->draw(shadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
    glViewport(0, 0, width, height);
    glClearColor(0.55f, 0.77, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    objectShader->use();
    model->setEnvironment(objectShader);
    objectShader->setUniform("shadowMap", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    model->draw(objectShader);

    glFlush();
}


std::vector<float> OBJET::GetDepthMap()
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
    // Make the FLOAT array
    GLfloat* pixels = new GLfloat[width * height];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);

    std::vector<float> image;
    float near = model->metaData.cameraPerspectiveNearPlane;
    float far = model->metaData.cameraPerspectiveFarPlane;
    for (int i = 0; i < width * height; i++) {
        float z = 2.0f * pixels[i] - 1.0f;
        z = (2.0 * near * far) / (far + near - z * (far - near));
        image.push_back(z);
    }

    return image;
}


std::vector<int> OBJET::GetImage()
{
    glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
    // Make the BYTE array
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
    glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);

    // Make the BYTE array
    GLubyte* pixels = new GLubyte[3 * width * height];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
    FreeImage_Save(FIF_PNG, image, pathToImage.c_str(), 0);
    FreeImage_Unload(image);
    delete [] pixels;
}


void OBJET::SetCamera(std::vector<float> position, std::vector<float> target)
{
    model->setCamera(position, target);
}


void OBJET::SetObjectPosition(std::string objectName, std::vector<float> position)
{
    model->setObjectPosition(objectName, position);
}


void OBJET::SetObjectYRotation(std::string objectName, float yRotation)
{
    model->setObjectYRotation(objectName, yRotation);
}


void OBJET::SetObjectScale(std::string objectName, float scale)
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
    window = glfwCreateWindow(width, height, "OBJET", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwHideWindow(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
      exit(1);

    // hide and caputure the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // rendering window size
    glViewport(0, 0, width, height);

    // assign callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // run depth check (Z-buffer)
    glEnable(GL_DEPTH_TEST);

    /* Reder FrameBuffer */ // off-screen rendering
    // weird stuff happens in i3 for example when rendering to
    // the main buffer, the desktop manager messes with window
    glGenFramebuffers(1, &renderFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer);
    // color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // renderbuffer object
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    /* Depth Map FrameBuffer */ // for shadowing
    glGenFramebuffers(1, &depthMapFramebuffer);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE);

}


void OBJET::InitShaders()
{
    objectShader = new Shader(objectVertexShader, objectFragmentShader);
    shadowShader = new Shader(shadowVertexShader, shadowFragmentShader);
}


void OBJET::InitModel(std::string pathToMetaJSON)
{
    data::MetaData metaData = data::MetaData(pathToMetaJSON.c_str());
    model = new Model(metaData);
}
