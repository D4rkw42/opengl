#include <iostream>
#include <thread>
#include <chrono>
 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/opengl/Shader.hpp"
#include "utils/opengl/VBO.hpp"
#include "utils/opengl/VAO.hpp"
#include "utils/opengl/EBO.hpp"

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

GLfloat vertices[] = {
    //              COORDINATES                         //      COLORS          
    -0.5f,      -0.5f * float(sqrt(3)) / 3,     0.0f,   1.0f,   0.0f,   0.0f,
    0.5f,       -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f,   0.3f,   0.02f,
    0.0f,       0.5f * float(sqrt(3)) * 2 / 3,  0.0f,   0.0f,   1.0f,   0.0f,
    -0.5f / 2,  0.5f * float(sqrt(3)) / 6,      0.0f,   0.9f,   0.45f,  0.17f,
    0.5f / 2,   0.5f * float(sqrt(3)) / 6,      0.0f,   0.9f,   0.45f,  0.17f,
    0.0f,       -0.5f * float(sqrt(3)) /3,      0.0f,   0.0f,   0.0f,   1.0f
};

GLuint indices[] {
    0, 3, 5,
    3, 2, 4,
    5, 4, 1
};

int main(void) {
    // Inicializa a biblioteca GLFW
    glfwInit();

    // Cria alguns "hints" para a operação do GLFW

    // Diz ao GLFW qual versão do biblioteca está sendo utilizada
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Diz ao GLFW qual profile do OpenGL está sendo utilizada
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criação da janela do GLFW
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);

    if (window == NULL) {
        std::cerr << "[GLFW] Unable to create window." << "\n";
        glfwTerminate();
    }

    // Diz ao GLFW qual janela será rederizada no momento
    glfwMakeContextCurrent(window);

    // Inicia as funções do OpenGL através do GLAD
    gladLoadGL();

    std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << "\n";

    // Diz ao OpenGL as dimensões de desenho dentro da janela
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Shader Básico
    Shader basicShader = Shader("basic_vertex_shader.vert", "basic_fragment_shader.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // obtendo o acesso à região uniform
    GLuint uniID = glGetUniformLocation(basicShader.ID, "scale");

    // Loop principal do aplicativo. Verifica se uma janela não requereu a parada da execução
    while (!glfwWindowShouldClose(window)) {
        // Carrega todos os eventos do GLFW  
        glfwPollEvents();

        // Seleciona uma cor de limpeza para a função de limpeza
        glClearColor(1.f, 1.f, 1.f, 1.f);
        // Função de limpeza com bufer do tipo COLOR_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // Utilizando programa
        basicShader.Activate();
        glUniform1f(uniID, 0.5f); // adicionando calor uniform no programa de shader

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Carrega todas as informações do back buffer para o front buffer (equivalente a renderPresent do SDL2)
        glfwSwapBuffers(window);
    }

    // Destruindo VAO, VBO e program
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    // Deletando shaders
    basicShader.Delete();

    // Destruição de janela e finalização da biblioteca GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
