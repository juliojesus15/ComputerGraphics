#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>    
#include <time.h>      

#include "generador_puntos.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Presionando la tecla A las figuras cambian de color
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mover_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int color_a = 0;
int color_b = 8;
int color_c = 20;

std::vector<float> rgb = {
    1.0f, 0.0f, 0.0f, 1.0f, // rojo
    0.0f, 1.0f, 0.0f, 1.0f, // verde 
    0.0f, 0.0f, 1.0f, 1.0f, // azul
    1.0f, 1.0f, 0.0f, 1.0f, // amarillo
    0.0f, 1.0f, 1.0f, 1.0f, // cyan
    1.0f, 0.0f, 1.0f, 1.0f, // magenta
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


int main()
{   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Computacion Grafica (Control) - UCSP", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //Generando puntos (Tam, Angulo, centro: x, y, z)
    Triangulo triangulo(0.2f, 90, -0.5f, 0.5f, 0.0f);
    std::vector<float> puntos_triangulo = triangulo.puntos;
    
    //Generando puntos (Radio, centro: x, y, z)
    Estrella estrella(0.5f, -0.5f, -0.5f, 0.0f);
    std::vector<float> puntos_estrella = estrella.puntos;    

    //Generando puntos (Radio, slice, centro: x, y, z)
    Pizza pizza(0.3f, 24, 0.5f, 0.6f, 0.0f);
    std::vector<float> puntos_pizza = pizza.puntos;    

    unsigned int VBO[3], VAO[3];
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    
    //Triangulo con lineas    
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * puntos_triangulo.size(), static_cast<void*>(puntos_triangulo.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //Generar estrella
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * puntos_estrella.size(), static_cast<void*>(puntos_estrella.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Generar Pizza
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * puntos_pizza.size(), static_cast<void*>(puntos_pizza.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);    

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Aumentar el tamanho de los puntos
    glPointSize(12);
    glLineWidth(8);

    // render loop
    // -----------    
    std::cout << "Presione la tecla A para cambiar de color" << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------        
        glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT
        );
        // Dibujando triangulos
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, rgb[color_a], rgb[color_a + 1], rgb[color_a + 2], rgb[color_a + 3]);
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_TRIANGLES, 0, (puntos_triangulo.size() / 3) - 1);

        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_TRIANGLES, 0, puntos_estrella.size() / 3 );

        glBindVertexArray(VAO[2]); // Pizza
        glDrawArrays(GL_TRIANGLES, 0, puntos_pizza.size() / 3);

        // Dibujando lineas
        glUniform4f(vertexColorLocation, rgb[color_b], rgb[color_b + 1], rgb[color_b + 2], rgb[color_b + 3]);
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_LINE_STRIP, 0, puntos_triangulo.size() / 3);

        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_LINE_STRIP, 0, puntos_estrella.size() / 3);
        
        glBindVertexArray(VAO[2]); // Pizza
        glDrawArrays(GL_LINE_STRIP, 0, puntos_pizza.size() / 3);


        // Dibujando puntos
        glUniform4f(vertexColorLocation, rgb[color_c], rgb[color_c + 1], rgb[color_c + 2], rgb[color_c + 3]);
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_POINTS, 0, (puntos_triangulo.size() / 3) - 1);

        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_POINTS, 0, puntos_estrella.size() / 3);

        glBindVertexArray(VAO[2]); // Pizza
        glDrawArrays(GL_POINTS, 0, puntos_pizza.size() / 3);

        
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(shaderProgram);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    srand(time(NULL));
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        color_a = (color_a + 4) % 24;
        color_b = (color_b + 4) % 24;
        color_c = (color_c + 4) % 24;
    }
}
