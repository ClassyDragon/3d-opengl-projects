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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GLCall.h"

void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main() {
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(700, 700, "Tetromino", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    glm::mat4 proj, view, model;
    glm::mat4 trans(1.0f);
    glm::mat4 perspectiveProjection = glm::perspective(glm::radians(40.0f), 500.0f / 500.0f, 0.1f, 100.f);
    proj = perspectiveProjection;
    view = glm::lookAt(glm::vec3(0, 4, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    model = glm::mat4(1.0f);

    // Camera
    glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    float degrees[] = {
        0.0f, 0.0f, 0.0f
    };
    float units[] = {
        0.0f, 0.0f, 0.0f
    };

    Cube cube(1.0f, "res/textures/crate.png");
    BatchCubes bc(&cube);
    bc.insertCube(glm::mat4(1.0));
    glm::mat4 trans1(1.0);
    trans1 = glm::translate(trans1, glm::vec3(2.0, 0.0, 0.0));
    bc.insertCube(trans1);
    glm::mat4 trans2(1.0);
    trans2 = glm::translate(trans2, glm::vec3(1.0, 2.0, 0.0));
    bc.insertCube(trans2);

    Shader shader("res/shaders/shader1.glsl");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(units[0], 0.0f, 0.0f));
        trans = glm::translate(trans, glm::vec3(0.0f, units[1], 0.0f));
        trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, units[2]));
//        trans = glm::rotate(trans, glm::radians(degrees[0]), glm::vec3(1.0f, 0.0f, 0.0f));
//        trans = glm::rotate(trans, glm::radians(degrees[1]), glm::vec3(0.0f, 1.0f, 0.0f));
//        trans = glm::rotate(trans, glm::radians(degrees[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, glm::radians(degrees[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(degrees[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(degrees[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.SetUniformMat4f("trans", trans);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 MVP = proj * view * model;
        shader.SetUniformMat4f("MVP", MVP);
        renderer.Clear();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderer.Draw(bc, shader);
        processInput(window, cameraPos, cameraFront, cameraUp);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Model Rotation");

        ImGui::SliderFloat3("Camera Pos", &cameraPos[0], -20.0f, 20.0f);
        ImGui::SliderFloat3("Camera Rotation", &degrees[0], 0.0f, 360.0f);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
