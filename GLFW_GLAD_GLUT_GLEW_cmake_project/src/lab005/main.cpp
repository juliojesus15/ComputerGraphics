#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>    
#include <time.h>      
#include <thread>
#include <chrono>

#include "triangulo.h"
#include "estrella.h"
#include "pizza.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Presionando la tecla A las figuras cambian de color
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Generando puntos (Tam, Angulo, centro: x, y, z)
Triangulo triangulo(0.2f, 90, 0.0f, 0.0f, 0.0f);

//Generando puntos (Radio, centro: x, y, z)
Estrella estrella(0.5f, -0.5f, -0.5f, 0.0f);

//Generando puntos (Radio, slice, centro: x, y, z)
Pizza pizza(0.3f, 24, 0.5f, 0.6f, 0.0f);

//Activar scala
bool activar_escala = false;
bool escala = true;

//Activar rotacion 
bool activar_rotacion = false;

// 1) Triangulo, 2) Estrella, 3) Pizza
int idx_figura = 1;

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
    //triangulo.test();
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Computacion Grafica (Lab005) - UCSP", NULL, NULL);
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
    unsigned int VBO[3], VAO[3];
    glGenVertexArrays(3, VAO);
    glGenBuffers(3, VBO);
    
    //Triangulo con lineas    
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangulo.puntos.size(), static_cast<void*>(triangulo.puntos.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //std::cout << "* " << triangulo.puntos.size() << std::endl;
    //std::cout << "* " << sizeof(float) * triangulo.puntos.size() << std::endl;
    //std::cout << "** " << static_cast<void*>(triangulo.puntos.data()) << std::endl;
    
    //Generar estrella
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * estrella.puntos.size(), static_cast<void*>(estrella.puntos.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Generar Pizza
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pizza.puntos.size(), static_cast<void*>(pizza.puntos.data()), GL_STATIC_DRAW);
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
    std::cout << "COLORES" << std::endl;
    std::cout << "1) Cambiar de color: PRESS KEY P" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << "ANIMACIONES" << std::endl;
    std::cout << "1) Activar rotación: PRESS KEY Q" << std::endl;
    std::cout << "2) Activar escala: PRESS KEY E" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << "PARA MOVER UNA FIGURA PRIMERO DEBE SELECCIONAR UNA FIGURA" << std::endl;
    std::cout << "1) Triangulo: PRESS KEY 1" << std::endl;
    std::cout << "2) Estrella: PRESS KEY 2" << std::endl;
    std::cout << "3) Pizza: PRESS KEY 3" << std::endl;
    std::cout << "=====================================================" << std::endl;
    std::cout << "CONTROLES" << std::endl;
    std::cout << "Arriba: PRES KEY UP" << std::endl;
    std::cout << "Abajo: PRES KEY DOWN" << std::endl;
    std::cout << "Izquierda: PRES KEY LEFT" << std::endl;
    std::cout << "Derecha: PRES KEY RIGHT" << std::endl;    
 
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
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangulo.puntos.size(), static_cast<void*>(triangulo.puntos.data()), GL_STATIC_DRAW);
                
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * estrella.puntos.size(), static_cast<void*>(estrella.puntos.data()), GL_STATIC_DRAW);       
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pizza.puntos.size(), static_cast<void*>(pizza.puntos.data()), GL_STATIC_DRAW);

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        glUniform4f(vertexColorLocation, rgb[color_a], rgb[color_a + 1], rgb[color_a + 2], rgb[color_a + 3]);
        glUseProgram(shaderProgram);
        
        if (activar_escala == true) {            
            if (idx_figura == 1) triangulo.aplicar_scala(!escala);
            else if (idx_figura == 2) estrella.aplicar_scala(!escala);
            else if (idx_figura == 3) pizza.aplicar_scala(!escala);
            //triangulo.aplicar_scala(escala);
            escala = !escala;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));                        
        }

        if (activar_rotacion == true) {   
            if (idx_figura == 1) triangulo.aplicar_rotacion(45.0f);
            else if (idx_figura == 2) estrella.aplicar_rotacion(45.0f);
            else if (idx_figura == 3) pizza.aplicar_rotacion(45.0f);
            //estrella.aplicar_rotacion(45.0f);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
               
        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_TRIANGLES, 0, (triangulo.puntos.size() / 3) - 1);
        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_TRIANGLES, 0, estrella.puntos.size() / 3 );
        glBindVertexArray(VAO[2]); // Pizza
        glDrawArrays(GL_TRIANGLES, 0, pizza.puntos.size() / 3);

        // Dibujando lineas
        glUniform4f(vertexColorLocation, rgb[color_b], rgb[color_b + 1], rgb[color_b + 2], rgb[color_b + 3]);
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_LINE_STRIP, 0, triangulo.puntos.size() / 3);
        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_LINE_STRIP, 0, estrella.puntos.size() / 3);        
        glBindVertexArray(VAO[2]); // Pizza
        glDrawArrays(GL_LINE_STRIP, 0, pizza.puntos.size() / 3);

        // Dibujando puntos
        glUniform4f(vertexColorLocation, rgb[color_c], rgb[color_c + 1], rgb[color_c + 2], rgb[color_c + 3]);
        glUseProgram(shaderProgram);
        

        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_POINTS, 0, (triangulo.puntos.size() / 3) - 1);
        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_POINTS, 0, estrella.puntos.size() / 3);
        glBindVertexArray(VAO[2]); // Pizza
        glDrawArrays(GL_POINTS, 0, pizza.puntos.size() / 3);
        
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
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        color_a = (color_a + 4) % 24;
        color_b = (color_b + 4) % 24;
        color_c = (color_c + 4) % 24;
    }

    //Elegir una figura
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS) idx_figura = 1;
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) idx_figura = 2;
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) idx_figura = 3;

    //Traslaciones
    else if (key == GLFW_KEY_W && action == GLFW_PRESS && idx_figura == 1) triangulo.aplicar_traslacion(0.0f, 0.1f, true);
    else if (key == GLFW_KEY_S && action == GLFW_PRESS && idx_figura == 1) triangulo.aplicar_traslacion(0.0f, -0.1f, true);
    else if (key == GLFW_KEY_D && action == GLFW_PRESS && idx_figura == 1) triangulo.aplicar_traslacion(0.1f, 0.0f, false);
    else if (key == GLFW_KEY_A && action == GLFW_PRESS && idx_figura == 1) triangulo.aplicar_traslacion(-0.1f, 0.0f, false);

    else if (key == GLFW_KEY_W && action == GLFW_PRESS && idx_figura == 2) estrella.aplicar_traslacion(0.0f, 0.1f, true);
    else if (key == GLFW_KEY_S && action == GLFW_PRESS && idx_figura == 2) estrella.aplicar_traslacion(0.0f, -0.1f, true);
    else if (key == GLFW_KEY_D && action == GLFW_PRESS && idx_figura == 2) estrella.aplicar_traslacion(0.1f, 0.0f, false);
    else if (key == GLFW_KEY_A && action == GLFW_PRESS && idx_figura == 2) estrella.aplicar_traslacion(-0.1f, 0.0f, false);

    else if (key == GLFW_KEY_W && action == GLFW_PRESS && idx_figura == 3) pizza.aplicar_traslacion(0.0f, 0.1f, true);
    else if (key == GLFW_KEY_S && action == GLFW_PRESS && idx_figura == 3) pizza.aplicar_traslacion(0.0f, -0.1f, true);
    else if (key == GLFW_KEY_D && action == GLFW_PRESS && idx_figura == 3) pizza.aplicar_traslacion(0.1f, 0.0f, false);
    else if (key == GLFW_KEY_A && action == GLFW_PRESS && idx_figura == 3) pizza.aplicar_traslacion(-0.1f, 0.0f, false);

    //Rotaciones
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) activar_rotacion = !activar_rotacion;

    //Escala
    else if (key == GLFW_KEY_E && action == GLFW_PRESS) activar_escala = !activar_escala;    
}


