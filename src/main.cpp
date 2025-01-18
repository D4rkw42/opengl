#include <iostream>
#include <thread>
#include <chrono>
 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/opengl/Shader.hpp"
#include "utils/opengl/VBO.hpp"
#include "utils/opengl/VAO.hpp"
#include "utils/opengl/EBO.hpp"
#include "utils/opengl/Camera.hpp"

using namespace std::literals::chrono_literals;

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;

GLfloat vertices[] = {
    //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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

    // Texture
    int imgWidth, imgHeight, numColCh;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* bytes = stbi_load("assets/images/brick.png", &imgWidth, &imgHeight, &numColCh, 0);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D); // gerando resoluções menores para ser usado caso necessário

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLint text0UniID = glGetUniformLocation(basicShader.ID, "tex0");
    basicShader.Activate();
    glUniform1i(text0UniID, 0);

    // habilitando o uso do deaph buffer
    glEnable(GL_DEPTH_TEST);

    // Objeto de câmera
    int windowWidth = WINDOW_WIDTH, windowHeight = WINDOW_HEIGHT;
    Camera camera = Camera(glm::vec3(0.0f, 0.0f, 2.0f), windowWidth, windowHeight);

    double prevTime = glfwGetTime();

    // Loop principal do aplicativo. Verifica se uma janela não requereu a parada da execução
    while (!glfwWindowShouldClose(window)) {
        // Carrega todos os eventos do GLFW  
        glfwPollEvents();

        double currTime = glfwGetTime();
        double delta = currTime - prevTime;
        prevTime = currTime;

        // obtendo o tamanho da janela
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        camera.width = windowWidth;
        camera.height = windowHeight;

        // Diz ao OpenGL as dimensões de desenho dentro da janela
        glViewport(0, 0, windowWidth, windowHeight);

        // Seleciona uma cor de limpeza para a função de limpeza
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Função de limpeza com bufer do tipo COLOR_BUFFER_BIT (formas geométricas) e DEPTH_BUFFER_BIT (profundidade)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Utilizando programa
        basicShader.Activate();

        // utilizando a câmera
        camera.Inputs(window, delta);
        camera.Matrix(45.0f, 0.1f, 100.0f, basicShader, "camMatrix");

        // Desenhando
        glBindTexture(GL_TEXTURE_2D, texture); // binding buffer

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Carrega todas as informações do back buffer para o front buffer (equivalente a renderPresent do SDL2)
        glfwSwapBuffers(window);

        // for convinience
        std::this_thread::sleep_for(1ms);
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
