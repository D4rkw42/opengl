#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 400;

int main(void) {
    // Inicializa a biblioteca GLFW
    glfwInit();
    

    // Cria alguns "hints" para a operação do GLFW

    // Diz ao GLFW qual versão do biblioteca está sendo utilizada
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Diz ao GLFW qual profile do OpenGL está sendo utilizada
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Criação da janela do GLFW
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "[GLFW] Unable to create window." << "\n";
        glfwTerminate();
    }

    // Diz ao GLFW qual janela será rederizada no momento
    glfwMakeContextCurrent(window);

    // Inicia as funções do OpenGL através do GLAD
    gladLoadGL();

    // Diz ao OpenGL as dimensões de desenho dentro da janela
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Seleciona uma cor de limpeza para a função de limpeza
    glClearColor(1.f, 1.f, 1.f, 1.f);

    // Loop principal do aplicativo. Verifica se uma janela não requereu a parada da execução
    while (!glfwWindowShouldClose(window)) {
        // Carrega todos os eventos do GLFW  
        glfwPollEvents();

        // Função de limpeza com bufer do tipo COLOR_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // Carrega todas as informações do back buffer para o front buffer (equivalente a renderPresent do SDL2)
        glfwSwapBuffers(window);
    }

    // Destruição de janela e finalização da biblioteca GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
