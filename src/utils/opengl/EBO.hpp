// definições gerais para EBO

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EBO {
    public:
        GLuint ID;

        EBO(GLuint* indices, GLsizeiptr size);

        void Bind(void);
        void Unbind(void);
        void Delete(void);

};
