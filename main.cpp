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
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

int main()
{
    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    GLFWwindow* window = glfwCreateWindow(500, 500, "LearnOpenGL", NULL, NULL);
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
    glViewport(0, 0, 500, 500);

    // assign callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // shader
    Shader* objectShader = new Shader("./shaders/vertex_shader.glsl", "./shaders/fragment_shader.glsl");

    // model
    data::MetaData metaData("./configs/meta.json");
    Model model(metaData);

    //  make transformations
    //  projection
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 500.0f / 500.0f, 0.1f, 1000.0f);

    // run depth check (Z-buffer)
    glEnable(GL_DEPTH_TEST);

    // render loop
    float time;
    float lastFrameTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClearColor(0.55f, 0.77, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // change color
        time = glfwGetTime();
        deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        // render objects with objectShader
        objectShader->use();
        objectShader->setUniform("projection", projection);
        objectShader->setUniform("cameraPosition", cameraPos);
            
        model.setEnvironmentPorperties(objectShader);
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
