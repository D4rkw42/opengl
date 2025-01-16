#include "Shader.hpp"

#include <iostream>
#include <fstream>

Shader::Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile) {
    const std::string vertexShaderContent = Shader::ReadShader(vertexShaderFile);
    const std::string fragmentShaderContent = Shader::ReadShader(fragmentShaderFile);

    const char* vertexShaderSource = vertexShaderContent.c_str();
    const char* fragmentShaderSource = fragmentShaderContent.c_str();

    // Gerando vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    CompileErrors(vertexShader, "VERTEX");

    // Gerando fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    CompileErrors(vertexShader, "FRAGMENT");

    // Criando shader program
    GLuint shaderProgram = glCreateProgram();

    // Linkando shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    CompileErrors(vertexShader, "PROGRAM");

    this->ID = shaderProgram;

    // Deletando shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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

// Manipulador de erros
void Shader::CompileErrors(GLuint shader, const char* type) {
    GLint hasCompiled;
    char infoLog[1024];

    if (strcmp(type, "PROGRAM") == 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

        if (hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "SHADER_COMPILATION_ERROR for: " << type << "\n";
            std::cerr << "reason: " << infoLog << "\n\n";
        }
    } else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);

        if (hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "SHADER_LINKING_ERROR for: " << type << "\n";
            std::cerr << "reason: " << infoLog << "\n\n";
        }
    }
}
