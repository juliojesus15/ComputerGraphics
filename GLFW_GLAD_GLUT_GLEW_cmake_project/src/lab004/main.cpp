#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "vitral.h"

//Definiendo vitral
Vitral vitral(1.6f, 1.4f, 0.0f, 0.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Trabajo 004: Vitral", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    unsigned int VBO[16], VAO[16], EBO[16];
    glGenVertexArrays(16, VAO);
    glGenBuffers(16, VBO);
    glGenBuffers(16, EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    //Background del cuadro
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.backgroung.size(), static_cast<void*>(vitral.backgroung.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_background.size(), static_cast<void*>(vitral.idx_background.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Margen del cuadro
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.backgroung.size(), static_cast<void*>(vitral.backgroung.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_margen.size(), static_cast<void*>(vitral.idx_margen.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tallo de la flor - background
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.tallo.size(), static_cast<void*>(vitral.tallo.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_tallo_background.size(), static_cast<void*>(vitral.idx_tallo_background.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tallo de la flor - margen
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.tallo.size(), static_cast<void*>(vitral.tallo.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_tallo_margen.size(), static_cast<void*>(vitral.idx_tallo_margen.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Petalo central - Background
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.petalo.size(), static_cast<void*>(vitral.petalo.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_petalo.size(), static_cast<void*>(vitral.idx_petalo.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // petalo central - margen
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.petalo.size(), static_cast<void*>(vitral.petalo.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Petalo izq - Background
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.petalo_izq.size(), static_cast<void*>(vitral.petalo_izq.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_petalo_izq.size(), static_cast<void*>(vitral.idx_petalo_izq.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // petalo izq - margen
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.petalo_izq.size(), static_cast<void*>(vitral.petalo_izq.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Petalo der - Background
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.petalo_der.size(), static_cast<void*>(vitral.petalo_der.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[8]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vitral.idx_petalo_der.size(), static_cast<void*>(vitral.idx_petalo_der.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // petalo der - margen
    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vitral.petalo_der.size(), static_cast<void*>(vitral.petalo_der.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Hoja izq - Background
    glBindVertexArray(VAO[10]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vitral.hoja_izq.size(), static_cast<void*>(vitral.hoja_izq.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[10]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* vitral.idx_hoja_izq.size(), static_cast<void*>(vitral.idx_hoja_izq.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // hoja der - margen
    glBindVertexArray(VAO[11]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vitral.hoja_izq.size(), static_cast<void*>(vitral.hoja_izq.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Hoja izq - Background
    glBindVertexArray(VAO[12]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vitral.hoja_der.size(), static_cast<void*>(vitral.hoja_der.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[12]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float)* vitral.idx_hoja_der.size(), static_cast<void*>(vitral.idx_hoja_der.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // hoja der - margen
    glBindVertexArray(VAO[13]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vitral.hoja_der.size(), static_cast<void*>(vitral.hoja_der.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(5);
    glLineWidth(3);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        //Background
        glUniform4f(vertexColorLocation, 0.0f, 0.28f, 0.89f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]); 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Marco
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0);

        // Tallo de la flor - background
        glUniform4f(vertexColorLocation, 0.61f, 0.42f, 0.25f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // tallo de la flor - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[3]);
        glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0);

        // Petalo central - background
        glUniform4f(vertexColorLocation, 0.63f, 0.78f, 0.75f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[4]);
        glDrawElements(GL_LINES, vitral.idx_petalo.size(), GL_UNSIGNED_INT, 0);

        // Petalo central - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[5]);
        glDrawArrays(GL_POINTS, 0, vitral.petalo.size() / 3);

        // Petalo izq - background
        glUniform4f(vertexColorLocation, 0.63f, 0.78f, 0.75f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[6]);
        glDrawElements(GL_LINES, vitral.idx_petalo_izq.size(), GL_UNSIGNED_INT, 0);

        // Petalo izq - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_POINTS, 0, vitral.petalo_izq.size() / 3);

        // Petalo der - background
        glUniform4f(vertexColorLocation, 0.63f, 0.78f, 0.75f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[8]);
        glDrawElements(GL_LINES, vitral.idx_petalo_der.size(), GL_UNSIGNED_INT, 0);

        // Petalo der - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[9]);
        glDrawArrays(GL_POINTS, 0, vitral.petalo_der.size() / 3);
        
        // Hoja izq - background
        glUniform4f(vertexColorLocation, 0.04f, 0.43f, 0.02f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[10]);
        glDrawElements(GL_LINES, vitral.idx_hoja_izq.size(), GL_UNSIGNED_INT, 0);

        // Hoja izq - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[11]);
        glDrawArrays(GL_POINTS, 0, vitral.hoja_izq.size() / 3);
        

        // Hoja der - background
        glUniform4f(vertexColorLocation, 0.04f, 0.43f, 0.02f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[12]);
        glDrawElements(GL_LINES, vitral.idx_hoja_der.size(), GL_UNSIGNED_INT, 0);

        // Hoja der - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[13]);
        glDrawArrays(GL_POINTS, 0, vitral.hoja_der.size() / 3);

        /*
        // Hoja der - background
        glUniform4f(vertexColorLocation, 0.04f, 0.43f, 0.02f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[14]);
        glDrawElements(GL_LINES, vitral.idx_puntos.size(), GL_UNSIGNED_INT, 0);

        // Hoja der - margen
        glUniform4f(vertexColorLocation, 0.14f, 0.13f, 0.14f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[15]);
        glDrawArrays(GL_POINTS, 0, vitral.puntos.size() / 3);
        */

        

        

        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(4, VAO);
    glDeleteBuffers(4, VBO);
    glDeleteBuffers(4, EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
