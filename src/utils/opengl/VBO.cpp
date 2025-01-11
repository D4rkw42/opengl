#include "VBO.hpp"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
    glGenBuffers(1, &this->ID); //cria um buffer (nesse caso será uma VBO). 1 significa que essa VBO está tratando de apenas 1 objeto 3D
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind(void) {
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
}

void VBO::Unbind(void) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete(void) {
    glDeleteBuffers(1, &this->ID);
}
