// Definições gerais para shaders

#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const std::string SHADERS_SOURCE_DIR = "/assets/shaders/";

class Shader {
    public:
        GLuint ID;

        Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

        // Ativa o shader para utilização no programa
        void Activate(void);

        // Deleta o shader
        void Delete(void);

        // utility
        static std::string ReadShader(const std::string& shaderFile); // faz a leitura de um shader

};
