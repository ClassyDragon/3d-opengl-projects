#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayoutData.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GLCall.h"

int main() {
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    window = glfwCreateWindow(500, 500, "Triangle Test", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    float vertices[] = {
        // positions         // tex coords
        /*
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 0
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 1

        -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, // 2
        -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, // 3

        -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // 4
        -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, // 5
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 6

         1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // 7
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, // 8
         1.0f,  1.0f, -1.0f, 1.0f, 0.0f, // 9

        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // 10
        -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, // 11
        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, // 12

         1.0f, -1.0f,  1.0f, 0.0f, 1.0f, // 13
         1.0f, -1.0f,  1.0f, 1.0f, 1.0f, // 14

         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 15
         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 16

         1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 17
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // 18
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // 19
         */
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 0
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 1

        -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, // 2
        -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, // 3

        -1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // 4
        -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, // 5
        -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 6

         1.0f,  1.0f, -1.0f, 1.0f, 1.0f, // 7
         1.0f,  1.0f, -1.0f, 0.0f, 0.0f, // 8
         1.0f,  1.0f, -1.0f, 1.0f, 0.0f, // 9

        -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, // 10
        -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, // 11
        -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, // 12

         1.0f, -1.0f,  1.0f, 0.0f, 1.0f, // 13
         1.0f, -1.0f,  1.0f, 1.0f, 1.0f, // 14

         1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // 15
         1.0f, -1.0f, -1.0f, 0.0f, 0.0f, // 16

         1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 17
         1.0f,  1.0f,  1.0f, 1.0f, 1.0f, // 18
         1.0f,  1.0f,  1.0f, 1.0f, 0.0f, // 19
    };

    unsigned int indices[] = {
        0, 2, 4,
        7, 0, 10,
        13, 1, 16,
        7, 15, 0,
        0, 4, 11,
        13, 3, 1,
        5, 2, 14,
        17, 15, 8,
        15, 17, 14,
        18, 9, 12,
        18, 12, 6,
        19, 5, 14
    };

    glm::mat4 proj, view, model;
    glm::mat4 trans(1.0f);
    glm::mat4 orthoProjection = glm::mat4(glm::ortho(0.0f, 500.0f, 0.0f, 500.0f, -1.0f, 1.0f));
//    proj = glm::perspective(glm::radians(90.0f), 500.0f / 500.0f, 0.1f, 100.0f) * glm::ortho(0.0f, 500.0f, 0.0f, 500.0f, -1.0f, 1.0f);
    glm::mat4 perspectiveProjection = glm::perspective(glm::radians(40.0f), 500.0f / 500.0f, 0.1f, 100.f);
//    proj = orthoProjection;
    proj = perspectiveProjection;
//    proj = orthoProjection * perspectiveProjection;
//    view = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
    view = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//    model = glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 250.0f, 0.0f)));
    //model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -4));
    float degrees[] = {
        0.0f, 0.0f, 0.0f
    };
    trans = glm::rotate(trans, glm::radians(degrees[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(degrees[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(degrees[2]), glm::vec3(0.0f, 0.0f, 1.0f));
//    trans = glm::translate(trans, glm::vec3(0, 0, -4));

    VertexBuffer vb(vertices, 5 * 20 * sizeof(float));
    vb.Bind();

    BufferLayoutData bl;
    bl.Insert<float>(3);
    bl.Insert<float>(2);

    VertexArray va;
    va.AddBuffer(vb, bl);
    va.Bind();

    IndexBuffer ib(indices, 12 * 3);
    ib.Bind();

    Texture texture("res/textures/pattern1.png");
    texture.Bind();

    Shader shader("res/shaders/shader1.glsl");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window)) {
        trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(degrees[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(degrees[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        trans = glm::rotate(trans, glm::radians(degrees[2]), glm::vec3(0.0f, 0.0f, 1.0f));
//        trans = glm::translate(trans, glm::vec3(0, 0, -4));
        shader.SetUniformMat4f("trans", trans);
        glm::mat4 MVP = proj * view * model;
        shader.SetUniformMat4f("MVP", MVP);
        renderer.Clear();
        glClear(GL_DEPTH_BUFFER_BIT);
        renderer.Draw(va, ib, shader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("3D");

        ImGui::SliderFloat3("Z Rotation", &degrees[0], 0.0f, 360.f);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
