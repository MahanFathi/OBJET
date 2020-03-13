#include <iostream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <streambuf>

#include "model.h"
#include "shader.h"
#include "util.h"
#include "common.h"

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// cam settings
float deltaTime = 0.0f;
glm::vec3 cameraPos   = glm::vec3(7.0f, 4.0f,  7.0f);
glm::vec3 cameraFront = glm::normalize(-cameraPos);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

int main()
{
    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const unsigned int width = 500, height = 500;

    // create a window
    GLFWwindow* window = glfwCreateWindow(width, height, "OBJET", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
      exit(1);

    // hide and caputure the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // rendering window size
    glViewport(0, 0, width, height);

    // assign callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // shader
    Shader* objectShader = new Shader("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");

    // model
    data::MetaData metaData("./configs/ps_meta.json");
    Model model(metaData);

    //  make transformations
    //  projection
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 500.0f / 500.0f, 0.1f, 1000.0f);

    // run depth check (Z-buffer)
    glEnable(GL_DEPTH_TEST);

    // shadowing
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    const unsigned int shadowWidth = 1024, shadowHeight = 1024;
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE); glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // shadow shader
    Shader* shadowShader = new Shader("./shaders/shadow_vertex_shader.glsl", "./shaders/shadow_fragment_shader.glsl");


    // render loop
    float time;
    float lastFrameTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        shadowShader->use();
        model.setShadow(shadowShader);
        glViewport(0, 0, shadowWidth, shadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        model.draw(shadowShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClearColor(0.55f, 0.77, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // change color
        time = glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        // render objects with objectShader
        objectShader->use();
        objectShader->setUniform("projection", projection);
        objectShader->setUniform("cameraPosition", cameraPos);
        model.setEnvironment(objectShader);
        objectShader->setUniform("shadowMap", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
           
        // overried camera settings
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        objectShader->setUniform("view", view);
        // draw
        model.draw(objectShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // cam adjustment
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool first = true;
    static float lastX;
    static float lastY;
    if (first) {
        lastX = xpos;
        lastY = ypos;
        first = false;
    }
    static float yaw{-90.0f};
    static float pitch{0.0f};
    float sensitivity = 0.05f;
    float offsetX =  xpos - lastX;
    float offsetY =  lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    yaw += offsetX * sensitivity;
    pitch += offsetY * sensitivity;
    if(pitch > 89.0f)
      pitch =  89.0f;
    if(pitch < -89.0f)
      pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
