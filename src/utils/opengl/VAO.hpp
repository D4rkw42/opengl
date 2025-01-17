// Definições para VAO

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/opengl/VBO.hpp"

class VAO {
    public:
        GLuint ID;

        VAO(void);

        void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind(void);
        void Unbind(void);
        void Delete(void);

};
