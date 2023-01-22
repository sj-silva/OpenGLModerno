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
#include"Shader.h"


unsigned int cubeVAO = 0;
unsigned int indexOffSet = 0;


glm::mat4 projection(1.f);
Shader shader("Main.vert", "Main.frag");


void CreateCube()
{

    glm::vec3 points[] =
    {
        // Front
        glm::vec3(-0.5f, +0.5f, +0.5f),//0
        glm::vec3(-0.5f, -0.5f, +0.5f),//1
        glm::vec3(+0.5f, -0.5f, +0.5f),//2
        glm::vec3(+0.5f, +0.5f, +0.5f),//3

        //Back
        glm::vec3(+0.5f, +0.5f, -0.5f),//4
        glm::vec3(+0.5f, -0.5f, -0.5f),//5
        glm::vec3(-0.5f, -0.5f, -0.5f),//6
        glm::vec3(-0.5f, +0.5f, -0.5f), //7

    };
    glm::vec3 vertices[] =
    {
        // Front
       points[0],//0
       points[1],//1
       points[2],//2 
       points[3],//3

       //Back
       points[4],//4 
       points[5],//5 
       points[6],//6 
       points[7],//7

       //Right
       points[3],//8
       points[2],//9 
       points[5],//10 
       points[4],//11

       //Left
       points[7],//12 
       points[6],//13 
       points[1],//14 
       points[0],//15

       //Top
       points[7],//16 
       points[0],//17 
       points[3],//18 
       points[4],//19

       //Bottom
       points[1],//20 
       points[6],//21 
       points[5],//22 
       points[2] //23	
    };

    GLubyte colors[] =
    {
        //Front Red
       255, 0, 0,
       255, 0, 0,
       255, 0, 0,
       255, 0, 0,

       //Back Green
       0, 255, 0,
       0, 255, 0,
       0, 255, 0,
       0, 255, 0,

       //Right Blue
        0, 0, 255,
        0, 0, 255,
        0, 0, 255,
        0, 0, 255,

        //Left Yellow
        255, 255, 0,
        255, 255, 0,
        255, 255, 0,
        255, 255, 0,

        //Top Magenta
        255, 0, 255,
        255, 0, 255,
        255, 0, 255,
        255, 0, 255,

        //Bottom Cyan
        0, 255, 255,
        0, 255, 255,
        0, 255, 255,
        0, 255, 255
    };

    unsigned int indices[] =
    {
        0,   1,  2,  0,  2,  3,// Front
        4,   5,  6,  4,  6,  7,// Back 
        8,   9, 10,  8, 10, 11,// Right 
        12, 13, 14, 12, 14, 15,// Left 
        16, 17, 18, 16, 18, 19,// Top 
        20, 21, 22, 20, 22, 23 // Bottom 
    };

    unsigned int BufferId = 0;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &BufferId);

    //The buffer is created but it will not be filled
    glBindBuffer(GL_ARRAY_BUFFER, BufferId);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(vertices) + sizeof(colors) + sizeof(indices), 0, GL_STATIC_DRAW);

    // Fill with vertices
    unsigned int currentOffSet = 0;
    glBufferSubData(GL_ARRAY_BUFFER, currentOffSet,
        sizeof(vertices), vertices);

    // Fill with colors
    currentOffSet += sizeof(vertices);
    glBufferSubData(GL_ARRAY_BUFFER, currentOffSet,
        sizeof(colors), colors);

    // Fill with indices
    currentOffSet += sizeof(colors);
    glBufferSubData(GL_ARRAY_BUFFER, currentOffSet,
        sizeof(indices), indices);


    indexOffSet = currentOffSet;

    glBindVertexArray(cubeVAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, BufferId);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE,
            3 * sizeof(GLubyte), (void*)sizeof(vertices));

    }
    glBindVertexArray(0);
}

void initOpenGL()
{

    CreateCube();

    shader.CreateShaders();
    shader.Bind();

    glEnable(GL_DEPTH_TEST);

    projection = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 10.f);
    projection = projection * glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.f));
}


void desenha(float dt)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::rotate(glm::mat4(1.f), -dt, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, 1.75f * dt, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, 0.75f * dt, glm::vec3(0.f, 0.f, 1.f));

    glm::mat4 finalMatrix = projection * model;

    shader.SendUniformData("Matrix", finalMatrix);

    glBindVertexArray(cubeVAO);
    {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)indexOffSet);
    }
    glBindVertexArray(0);

}



int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "3D Cube", NULL, NULL);
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


    float startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        desenha(dt);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}