#include<gl/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<string>
#include<fstream>
#include<sstream>

#include"Error.h"


unsigned int programId;

std::string ReadProgramSource(const std::string filePath)
{
    std::fstream file(filePath);

    std::stringstream ss;
    std::string line;

    while (getline(file, line))
    {
        ss << line << "\n";
    }
    file.close();

    return ss.str();
}

void CompileAndLinkShaders()
{
    // 1. Criamos os nossos Objetos: 
    //    Programa = Vertex Shader + Fragment Shader    
    programId = glCreateProgram();
    unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // 2. Passamos as strings para com códigos GLSL
    //    para o tipo const char* = código fonte final
    std::string vsCode, fsCode;
    vsCode = ReadProgramSource("Main.vert");
    fsCode = ReadProgramSource("Main.frag");

    const char* vsFinalCode = vsCode.c_str();
    const char* fsFinalCode = fsCode.c_str();

    // 3. Copiamos o código fonte final 
    //para o Shader anteriormente criado
    glShaderSource(vertexShaderId, 1, &vsFinalCode, NULL);
    glShaderSource(fragmentShaderId, 1, &fsFinalCode, NULL);

    // 4. Compilamos os Shaders
    glCompileShader(vertexShaderId);
    glCompileShader(fragmentShaderId);

    // 5. Anexamos os Shaders compilados ao Programa
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);


    //6. Link
    glLinkProgram(programId);

    //7. Delete
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);


    //8. Utilizar o programa
    glUseProgram(programId);

}

void initOpenGL()
{
    float vertices[] =
    {
         +1.f,        +0.f , //+0.0f, +0.5f,
        -0.5f, +sqrt(3) / 2.f, //- 0.5f, -0.5f,
        -0.5f, -sqrt(3) / 2.f, //-0.5f, - 0.5f
    };
    
    GLubyte colors[] =
    {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255
    };

    unsigned int positionVBO = 0, colorVBO=0; 

    glGenBuffers(1, &positionVBO);
    glGenBuffers(1, &colorVBO);
    
    //Ajustar position VBO
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Ajustar Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3*sizeof(GLubyte), 0);
    glEnableVertexAttribArray(1);

    CompileAndLinkShaders();
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Triangle - Color VBO", NULL, NULL);
    if (!window)
    {
        fatalError("GLFW Window could not be created!");
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fatalError("Error loading GLEW extensions!");
    }

    initOpenGL();


    //int colorId = glGetUniformLocation(programId, "u_deltaTime");
    int matrixId = glGetUniformLocation(programId, "Matrix");

    float startTime = glfwGetTime();

    glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.f), glm::vec3(0.5f, 0.5f, 1.f));

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        //glUniform1f(colorId, dt);
        glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.f), glm::radians(60.f * dt), glm::vec3(0.f, 0.f, 1.f));
        
        glm::mat4 FinalTransMatrix = RotationMatrix * ScaleMatrix;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(FinalTransMatrix));

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
