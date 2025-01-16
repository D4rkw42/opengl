#include <iostream>
#include <thread>
#include <chrono>
 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "utils/opengl/Shader.hpp"
#include "utils/opengl/VBO.hpp"
#include "utils/opengl/VAO.hpp"
#include "utils/opengl/EBO.hpp"

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

GLfloat vertices[] = {
    //              COORDINATES     //      COLORS          // TEXTURE COORDINATE //  
    -0.3f, -0.3f, 0.0f,             1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
    -0.3f, 0.3f, 0.0f,              0.0f, 1.0f, 0.0f,       0.0f, 1.0f,
    0.3f, 0.3f, 0.0f,               0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
    0.3f, -0.3f, 0.0f,              1.0f, 1.0f, 1.0f,       1.0f, 0.0f
};

GLuint indices[] {
    0, 2, 1,
    0, 3, 2
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

    // Shader Básico
    Shader basicShader = Shader("basic_vertex_shader.vert", "basic_fragment_shader.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // obtendo o acesso à região uniform
    GLuint uniID = glGetUniformLocation(basicShader.ID, "scale");

    // Texture
    int imgWidth, imgHeight, numColCh;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* bytes = stbi_load("assets/images/texture.jpg", &imgWidth, &imgHeight, &numColCh, 0);

    GLuint texture;
    glGenTextures(1, &texture); // gerando buffer
    glActiveTexture(GL_TEXTURE0); // ativando textura no slot 0 (16 max)
    glBindTexture(GL_TEXTURE_2D, texture); // binding buffer

    // modificação de atributos

    // filtro/modo de desenho -> LINEAR (blur scaling) ou NEAREST (same pixels while scaling)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // gerando textura para o uso do opengl
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D); // gerando resoluções menores para ser usado caso necessário

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint text0UniID = glGetUniformLocation(basicShader.ID, "tex0");
    basicShader.Activate();
    glUniform1i(text0UniID, 0);

    // Loop principal do aplicativo. Verifica se uma janela não requereu a parada da execução
    while (!glfwWindowShouldClose(window)) {
        // Carrega todos os eventos do GLFW  
        glfwPollEvents();

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Diz ao OpenGL as dimensões de desenho dentro da janela
        glViewport(0, 0, windowWidth, windowHeight);

        // Seleciona uma cor de limpeza para a função de limpeza
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Função de limpeza com bufer do tipo COLOR_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // Utilizando programa
        basicShader.Activate();
        glUniform1f(uniID, 0.5f); // adicionando calor uniform no programa de shader
        glBindTexture(GL_TEXTURE_2D, texture); // binding buffer

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Carrega todas as informações do back buffer para o front buffer (equivalente a renderPresent do SDL2)
        glfwSwapBuffers(window);
    }

    // Destruindo VAO, VBO e program
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    // Deletando shaders
    basicShader.Delete();

    // Deletando texturas
    glDeleteTextures(1, &texture);

    // Destruição de janela e finalização da biblioteca GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
