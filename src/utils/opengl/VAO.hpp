// Definições para VAO

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/opengl/VBO.hpp"

class VAO {
    public:
        GLuint ID;

        VAO(void);

        void LinkVBO(VBO VBO, GLuint layout);
        void Bind(void);
        void Unbind(void);
        void Delete(void);

};
