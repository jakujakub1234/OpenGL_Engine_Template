#include "../../include/engine_core/GLCore.h"
#include "../../include/engine_core/ShaderManager.h"
#include "../../include/utils/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

CameraManager GLCore::cameraManager = CameraManager();

float GLCore::last_x = GLCore::SCR_WIDTH / 2.0;
float GLCore::last_y = GLCore::SCR_HEIGHT / 2.0;
bool GLCore::first_mouse = true;

GLCore::GLCore()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    stbi_set_flip_vertically_on_load(true);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Engine Template", NULL, NULL);
    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, this->FramebufferSizeCallback);
    glfwSetCursorPosCallback(this->window, this->MouseCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->platform_shader_manager = ShaderManager();
    this->platform_shader_manager.BuildShaders((char*)"shaders/PlatformVertexShader.vs", (char*)"shaders/PlatformFragmentShader.fs");

    this->BindObjects();

    this->texture_manager = TextureManager();

    this->platform_texture = this->texture_manager.LoadTexture((char*)"textures/placeholder.jpg", false);

    this->platform_shader_manager.Use();
    this->platform_shader_manager.SetInt((char*)"texture1", 0);
}

void GLCore::BindObjects()
{
    glGenVertexArrays(1, &this->platform_VAO);
    glGenBuffers(1, &this->platform_VBO);
    glBindVertexArray(this->platform_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->platform_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->platform_vertices), this->platform_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLCore::DrawPlatforms()
{
    glm::mat4 projection = glm::perspective(glm::radians(this->cameraManager.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = this->cameraManager.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->platform_texture);

    this->platform_shader_manager.Use();
    this->platform_shader_manager.SetMat4((char*)"projection", projection);
    this->platform_shader_manager.SetMat4((char*)"view", view);
    this->platform_shader_manager.SetFloat((char*)"time", static_cast<float>(glfwGetTime()));

    glBindVertexArray(platform_VAO);

    model = glm::mat4(1.0f);
    platform_shader_manager.SetMat4((char*)"model", model);

    this->platform_shader_manager.SetFloat((char*)"transparent", 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

void GLCore::RenderLoop()
{
    int prev_sec = 0;
    int counter = 0;

    while (!glfwWindowShouldClose(this->window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        this->delta_time = current_frame - this->last_frame;
        this->last_frame = current_frame;

        if (current_frame > prev_sec) {
            prev_sec += 1;
            std::cout << "FPS: " << counter << std::endl;
            counter = 0;
        }
        counter++;

        this->ProcessInput();

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->DrawPlatforms();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &this->platform_VAO);
    glDeleteBuffers(1, &this->platform_VBO);

    this->platform_shader_manager.DeleteShader();

    glfwTerminate();
}

void GLCore::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->cameraManager.ProcessKeyboard(CAM_FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->cameraManager.ProcessKeyboard(CAM_BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->cameraManager.ProcessKeyboard(CAM_LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->cameraManager.ProcessKeyboard(CAM_RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        this->cameraManager.ProcessKeyboard(CAM_DOWN, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        this->cameraManager.ProcessKeyboard(CAM_UP, delta_time);
    }

    this->cameraManager.UpdateCameraVectors();
}

void GLCore::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLCore::MouseCallback(GLFWwindow* window, double xpos_in, double ypos_in)
{
    float xpos = static_cast<float>(xpos_in);
    float ypos = static_cast<float>(ypos_in);

    if (GLCore::first_mouse)
    {
        GLCore::last_x = xpos;
        GLCore::last_y = ypos;
        GLCore::first_mouse = false;
    }

    float xoffset = xpos - GLCore::last_x;
    float yoffset = GLCore::last_y - ypos;

    GLCore::last_x = xpos;
    GLCore::last_y = ypos;

    cameraManager.ProcessMouseMovement(xoffset, yoffset);
}