#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLCall.h"

enum MovementMode { WALK, FLY };

class PerspectiveCamera {
    public:
        PerspectiveCamera(const int cameraMode);
        ~PerspectiveCamera();
        void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        void updateCamera(GLFWwindow* window, float deltaTime);
        glm::mat4 getLookAtMatrix();
    private:
        glm::vec3 cameraPos, cameraFront, cameraUp;
        float yaw, pitch;
        float lastX, lastY;
        float deltaTime, lastFrame;
        bool firstMouse;
        int mode;
    public:
        static float sensitivity;
        static float mouseXPos;
        static float mouseYPos;
};
