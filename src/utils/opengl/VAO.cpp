#include "VAO.hpp"

VAO::VAO(void) {
    glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO VBO, GLuint layout) {
    VBO.Bind();

    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
