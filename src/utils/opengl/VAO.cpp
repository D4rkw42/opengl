#include "VAO.hpp"

VAO::VAO(void) {
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.Bind();

    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    VBO.Unbind();
}

void VAO::Bind(void) {
    glBindVertexArray(this->ID);
}

void VAO::Unbind(void) {
    glBindVertexArray(0);
}

void VAO::Delete(void) {
    glDeleteVertexArrays(1, &this->ID);
}
