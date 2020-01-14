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

int main() {
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(screenWidth, screenHeight, "Crates", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    PerspectiveCamera camera(1);
    glfwSetCursorPosCallback(window, mouseCallback);

    glewInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

    glm::mat4 proj, view, model;
    glm::mat4 trans(1.0f);
    glm::mat4 perspectiveProjection = glm::perspective(glm::radians(40.0f), screenWidth / screenHeight, 0.1f, 100.f);
    proj = perspectiveProjection;
    view = glm::lookAt(glm::vec3(0, 4, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    model = glm::mat4(1.0f);

    Model cubeModel("res/models/YoungLink.obj");

    float degrees[] = {
        0.0f, 0.0f, 0.0f
    };
    float units[] = {
        0.0f, 0.0f, 0.0f
    };

    Cube cube(1.0f, "res/textures/crate.png", 0);
    BatchCubes bc(&cube);
    bc.insertCube(glm::mat4(1.0));
    glm::mat4 trans1(1.0);
    trans1 = glm::translate(trans1, glm::vec3(2.2, 0.0, 0.0));
    bc.insertCube(trans1);
    glm::mat4 trans2(1.0);
    trans2 = glm::translate(trans2, glm::vec3(1.0, 2.1, 0.0));
    bc.insertCube(trans2);

    float floorVertices[] = {
        -20.0f, -1.0f, -20.0f, 0.0f, 0.0f,
        -20.0f, -1.0f,  20.0f, 0.0f, 1.0f,
         20.0f, -1.0f, -20.0f, 1.0f, 0.0f,
         20.0f, -1.0f,  20.0f, 1.0f, 1.0f
    };
    unsigned int floorIndices[] = {
        0, 1, 2,
        1, 2, 3
    };
    VertexBuffer vb(floorVertices, 7 * 4 * sizeof(float));
    IndexBuffer ib(floorIndices, 2 * 3);
    BufferLayoutData bf;
    bf.Insert<float>(3);
    bf.Insert<float>(2);
    VertexArray va;
    va.Bind();
    va.AddBuffer(vb, bf);

    Texture crate("res/textures/crate.png");

    Shader shader("res/shaders/shader1.glsl");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Cube grassBlock(1.0f, "res/textures/grassTexture.png", 1);

    Renderer renderer;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();
        glClear(GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.updateCamera(window, deltaTime);

        view = camera.getLookAtMatrix();
        glm::mat4 MVP = proj * view * model;

        shader.Bind();
        shader.SetUniformMat4f("MVP", MVP);

//        renderer.Draw(bc, shader);
        crate.Bind();
        renderer.Draw(cubeModel, shader);

        crate.Bind();
        glm::mat4 trans2(1.0f);
        shader.Bind();
        shader.SetUniformMat4f("trans", trans2);
    //    renderer.Draw(va, ib, shader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Model Rotation");

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
