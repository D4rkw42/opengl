// Definições para câmera

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/opengl/Shader.hpp"

class Camera {
    public:
        glm::vec3 position;
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        int width, height;

        float speed = 4.0f;
        float sensivity = 105.0f;

        bool firstClick = true;

        Camera(void) {};
        Camera(const glm::vec3& position, int width, int height);

        void Matrix(float FOVdeg, float nearPlane, float farPlane, const Shader& shader, const char* uniform);
        void Inputs(GLFWwindow* window, float delta);

};
