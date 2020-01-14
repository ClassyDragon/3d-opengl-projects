#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cmath>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayoutData.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Cube.h"
#include "Model.h"

#include "PerspectiveCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GLCall.h"

const float screenWidth = 1000.0f;
const float screenHeight = 1000.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    PerspectiveCamera::mouseXPos = xpos;
    PerspectiveCamera::mouseYPos = ypos;
}

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "[Error!] No model file specified!" << std::endl
            << "\t  Usage: " << argv[0] << " [model filepath] [scale]" << std::endl
            << "\tExample: " << argv[0] << " res/models/model.obj 0.75" << std::endl;
        return 1;
    }

    std::cout << "Press [SPACE] to toggle wireframe mode!" << std::endl;

    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(screenWidth, screenHeight, argv[1], NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    PerspectiveCamera camera(1);
    camera.setPosition(glm::vec3(0.0f, 5.5f, 10.0f));
    glfwSetCursorPosCallback(window, mouseCallback);

    glewInit();

    /*
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    */

    glm::mat4 proj, view, model;
    glm::mat4 perspectiveProjection = glm::perspective(glm::radians(70.0f), screenWidth / screenHeight, 0.1f, 1000.f);
    proj = perspectiveProjection;
    model = glm::mat4(1.0f);

    float scale = 1.0f;
    if (argc > 2) {
        scale = std::stof(argv[2], nullptr);
    }
    model = glm::scale(model, glm::vec3(scale, scale, scale));

    Model model3D(argv[1]);
    if (!model3D.Loaded()) {
        glfwTerminate();
        return 1;
    }

    Texture colorful("res/textures/colors.png");
    colorful.Bind();

    Shader shader("res/shaders/shader1.glsl");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.updateCamera(window, deltaTime);

        view = camera.getLookAtMatrix();
        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 MVP = proj * view * model;

        colorful.Bind();
        shader.Bind();
        shader.SetUniformMat4f("MVP", MVP);

        renderer.Draw(model3D, shader);

        /*
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Model Rotation");

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
