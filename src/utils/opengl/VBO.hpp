// definições gerais para VBO

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO {
    public:
        GLuint ID;

        VBO(GLfloat* vertices, GLsizeiptr size);

        void Bind(void);
        void Unbind(void);
        void Delete(void);

};
