#include <iostream>
#include <thread>
#include <chrono>
 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;

// Estipulação de um shader de vértices simples. Orienta a GPU para a localização dos pontos de desenho
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Estipulação de um shader fragment simples. Orienta a GPU sobre questões de colozização, trabnsparência, luz e sombra
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

// Primeiro a GPU processa os vértices para desenho e, após isso, aplica o fragment shader para colorir, definir transparência, luz e sombras

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

    // Diz ao OpenGL as dimensões de desenho dentro da janela
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLfloat vertices[] = {
        -0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,
        0.5f,   -0.5f * float(sqrt(3)) / 3,     0.0f,
        0.0f,   0.5f * float(sqrt(3)) * 2 / 3,  0.0f
    };


    // Cria um shader para vértices
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Carrega o shader que criamos em vertexShaderSource
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compila o Shader para machine code para que a GPU possa processar
    glCompileShader(vertexShader);


    // Cria um shader fragment
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Carrega o shader que criamos em vertexShaderSource
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compila o Shader para machine code para que a GPU possa processar
    glCompileShader(fragmentShader);

    // Cria um shader program, que une diversos shaders para operação conjunta
    GLuint shaderProgram = glCreateProgram();

    // Linkando os devidos shaders ao programa
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Linka os shaders propriamente
    glLinkProgram(shaderProgram);

    // Deletando os shaders (vamos usar a compilação conjunta apenas!)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    // Carregar dados da CPU para a GPU é lento! Devemos, então, mandar todos os dados compilados em grandes pacotes para ser mais otimizado

    // Vertex Array Object: une diversos VBO's para rápido processamento
    // Vertex Buffer Object: pacote de dados para indicar à GPU os vértices
    GLuint VAO, VBO; // Perceba que, em vez de uma array de vértices, o que será carregado na GPU será apenas um unsigned int representando bits de renderização

    // Gerando VAO, trabalhando com 1 objeto
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // cria um buffer (nesse caso será uma VBO). 1 significa que essa VBO está tratando de apenas 1 objeto 3D

    glBindVertexArray(VAO); // bindinng VAO
    // Bind é dizer para o OpenGL qual objeto está sendo trabalhado no momento. Todas as funções que acessam objetos irão acessá-lo até qque seja trocado
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // gerando os dados para a VBO. Além do tipo de buffer, do tamanho do vetor de vértices e dos vértices, a função precisa saber como a informação irá trabalhar
    // GL_STATIC -> VBO será modificado uma vez e usado várias vezes
    // GL_DYNAMIC -> VBO será modificado várias vezes e usado várias vezes
    // DRAW por que iremos desenhar, pras poderíamos fazer operações de COPY e READ também
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // Loop principal do aplicativo. Verifica se uma janela não requereu a parada da execução
    while (!glfwWindowShouldClose(window)) {
        // Carrega todos os eventos do GLFW  
        glfwPollEvents();

        // Seleciona uma cor de limpeza para a função de limpeza
        glClearColor(1.f, 1.f, 1.f, 1.f);
        // Função de limpeza com bufer do tipo COLOR_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // Utilizando programa
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Carrega todas as informações do back buffer para o front buffer (equivalente a renderPresent do SDL2)
        glfwSwapBuffers(window);

        // for convenience
        std::this_thread::sleep_for(std::chrono::nanoseconds(1000000));
    }

    // Destruindo VAO, VBO e program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Destruição de janela e finalização da biblioteca GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
