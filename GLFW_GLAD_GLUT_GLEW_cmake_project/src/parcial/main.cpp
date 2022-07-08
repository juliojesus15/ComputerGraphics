/*
* Alumno: Julio Ticona Quispe
* Curso: Computacion Grafica - Parcial practico
*/
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
Estrella estrella(0.2f, -0.8f, -0.8f, 0.0f);

Cuadrado cuadrado(0.2f, 0.2f, 0.3f, 0.0f);

Rombo rombo(0.2f, 0.6f, 0.6f, 0.0f);

//Estrella animacion
bool eje_estrella = false;
bool orientacion_estrella = true;
double pasos_estrella = 15;

//Rombo animacion
bool eje = false;
bool orientacion = false;
double pasos = 12;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Computacion Grafica (PARCIAL PRACTICO) - UCSP", NULL, NULL);
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
    unsigned int VBO[4], VAO[4];
    glGenVertexArrays(4, VAO);
    glGenBuffers(4, VBO);
    
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

    //Generar Cuadrado
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cuadrado.puntos.size(), static_cast<void*>(cuadrado.puntos.data()), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);    

    //Generar rombo
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rombo.puntos.size(), static_cast<void*>(rombo.puntos.data()), GL_STATIC_DRAW);
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
    glPointSize(8);
    glLineWidth(3);

    std::cout << "MENU" << std::endl;
    std::cout << "Presiona P para cambiar el color de los vertices" << std::endl;
    
      
 
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cuadrado.puntos.size(), static_cast<void*>(cuadrado.puntos.data()), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rombo.puntos.size(), static_cast<void*>(rombo.puntos.data()), GL_STATIC_DRAW);

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        glUniform4f(vertexColorLocation, rgb[color_a], rgb[color_a + 1], rgb[color_a + 2], rgb[color_a + 3]);
        glUseProgram(shaderProgram);
        
        //Escala triangulo
        triangulo.aplicar_scala(!escala);        
        escala = !escala;
        //std::this_thread::sleep_for(std::chrono::milliseconds(300));
        
        //Rotacion cuadrado
        cuadrado.aplicar_rotacion(25.0f);


        //Traslacion rombo
        if (eje == false && orientacion == false) {
            rombo.aplicar_traslacion(-0.1f, 0.0f, false);
            pasos = pasos - 1;
            //std::cout << pasos << std::endl;
            if (pasos == 0) {
                eje = true;
                orientacion = false;
                pasos = 9;
            }
        }

        else if (eje == true && orientacion == false) {
            rombo.aplicar_traslacion(0.0f, -0.1f, true);
            pasos = pasos - 1;
            //std::cout << pasos << std::endl;
            if (pasos == 0) {
                eje = false;
                orientacion = true;
                pasos = 12;
            }
        }

        else if (eje == false && orientacion == true) {
            rombo.aplicar_traslacion(0.1f, 0.0f, false);
            pasos = pasos - 1;
            //std::cout << pasos << std::endl;
            if (pasos == 0) {
                eje = true;
                orientacion = true;
                pasos = 9;
            }
        }
        else if (eje == true && orientacion == true) {
            rombo.aplicar_traslacion(0.0f, 0.1f, true);
            pasos = pasos - 1;
            //std::cout << pasos << std::endl;
            if (pasos == 0) {
                eje = false;
                orientacion = false;
                pasos = 12;
            }
        }


        //Animacion Estrella        
        //estrella.aplicar_rotacion(1.0f);
        if (eje_estrella == false && orientacion_estrella == true) {
            estrella.aplicar_traslacion(0.1f, 0.0f, false);
            estrella.aplicar_rotacion(90.0f);
            pasos_estrella = pasos_estrella - 1;
            //std::cout << pasos << std::endl;
            if (pasos_estrella == 0) {
                eje_estrella = true;
                orientacion_estrella = true;
                pasos_estrella = 15;
            }
        }

        else if (eje_estrella == true && orientacion_estrella == true) {
            estrella.aplicar_traslacion(0.0f, 0.1f, true);
            estrella.aplicar_rotacion(90.0f);
            pasos_estrella = pasos_estrella - 1;
            //std::cout << pasos << std::endl;
            if (pasos_estrella == 0) {
                eje_estrella = false;
                orientacion_estrella = false;
                pasos_estrella = 15;
            }
        }
    
        else if (eje_estrella == false && orientacion_estrella == false) {
            estrella.aplicar_traslacion(-0.1f, 0.0f, false);
            estrella.aplicar_rotacion(90.0f);
            pasos_estrella = pasos_estrella - 1;
            //std::cout << pasos << std::endl;
            if (pasos_estrella == 0) {
                eje_estrella = true;
                orientacion_estrella = false;
                pasos_estrella = 15;
            }
        }
        
        else if (eje_estrella == true && orientacion_estrella == false) {
            estrella.aplicar_traslacion(0.0f, -0.1f, true);
            estrella.aplicar_rotacion(90.0f);
            pasos_estrella = pasos_estrella - 1;
            //std::cout << pasos << std::endl;
            if (pasos_estrella == 0) {
                eje_estrella = false;
                orientacion_estrella = true;
                pasos_estrella = 12;
            }
        }
        

        std::this_thread::sleep_for(std::chrono::milliseconds(250));

               
        glUniform4f(vertexColorLocation,255.0f/ 255.0f,0.0f, 0.0f, 0.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_TRIANGLES, 0, (triangulo.puntos.size() / 3) - 1);

        glUniform4f(vertexColorLocation, 255.0f / 255.0f, 204.0f / 255.0f, 102.0f / 255.0f, 0.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_TRIANGLES, 0, estrella.puntos.size() / 3 );

        glUniform4f(vertexColorLocation, 146.0f / 255.0f, 208.0f / 255.0f, 80.0f / 255.0f, 0.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[2]); // Cuadrado
        glDrawArrays(GL_TRIANGLES, 0, cuadrado.puntos.size() / 3);

        glUniform4f(vertexColorLocation, 0.0f, 51.0f / 255.0f, 204.0f / 255.0f, 0.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[3]); // Rombo
        glDrawArrays(GL_TRIANGLES, 0, rombo.puntos.size() / 3);
        

        // Dibujando lineas
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 0.0f);
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_LINE_STRIP, 0, triangulo.puntos.size() / 3);
        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_LINE_STRIP, 0, estrella.puntos.size() / 3);        
        glBindVertexArray(VAO[2]); // Cuadrado
        glDrawArrays(GL_LINE_STRIP, 0, cuadrado.puntos.size() / 3);
        glBindVertexArray(VAO[3]); // Cuadrado
        glDrawArrays(GL_LINE_STRIP, 0, rombo.puntos.size() / 3);
        

        // Dibujando puntos
        glUniform4f(vertexColorLocation, rgb[color_c], rgb[color_c + 1], rgb[color_c + 2], rgb[color_c + 3]);
        glUseProgram(shaderProgram);        

        glBindVertexArray(VAO[0]); // Triangulo
        glDrawArrays(GL_POINTS, 0, (triangulo.puntos.size() / 3) - 1);
        glBindVertexArray(VAO[1]); // Estrella
        glDrawArrays(GL_POINTS, 0, estrella.puntos.size() / 3);
        glBindVertexArray(VAO[2]); // Cuadrado
        glDrawArrays(GL_POINTS, 0, cuadrado.puntos.size() / 3);
        glBindVertexArray(VAO[3]); // Rombo
        glDrawArrays(GL_POINTS, 0, rombo.puntos.size() / 3);
        
        
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
}
