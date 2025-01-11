#include "Shader.hpp"

#include <iostream>
#include <fstream>

Shader::Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile) {
    const char* vertexShaderSource = Shader::ReadShader(vertexShaderFile).c_str();
    const char* fragmentShaderSource = Shader::ReadShader(fragmentShaderFile).c_str();

    // Gerando vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Gerando fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Criando shader program
    GLuint shaderProgram = glCreateProgram();

    // Linkando shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    this->ID = shaderProgram;

    // Deletando shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //delete[] vertexShaderSource;
    //delete[] fragmentShaderSource;
}

void Shader::Activate(void) {
    glUseProgram(this->ID);
}

void Shader::Delete(void) {
    glDeleteProgram(this->ID);
}

//

std::string Shader::ReadShader(const std::string& shaderFile) {
    std::ifstream file;
    std::string content;

    std::string line;

    file.open("." + SHADERS_SOURCE_DIR + shaderFile);

    while (std::getline(file, line)) {
        content += line + "\n";
    }

    return content;
}
