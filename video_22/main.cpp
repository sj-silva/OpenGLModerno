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
//#include"Cube.h"

GLFWwindow* window;
unsigned int VAO;

void initOpenGL()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        fatalError("GLFW could not be initialized");
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "3D Cube", NULL, NULL);
    if (!window)
    {
        fatalError("GLFW Window could not be created!");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fatalError("Error loading GLEW extensions!");
    }

    glEnable(GL_DEPTH_TEST);
}

void CreateQuad()
{
    unsigned int EBO = 0;
    unsigned int positionVBO = 0;
    unsigned int colorVBO = 0;
    unsigned int centerOffSetVBO = 0;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &positionVBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &centerOffSetVBO);

    glm::vec2 vertices[] =
    {
        glm::vec2(+1.f, +0.f),//0
        glm::vec2(+0.f, +1.f),//1
        glm::vec2(-1.f, +0.f),//2
        glm::vec2(+0.f, -1.f) //3
    };

    GLubyte colors[] =
    {
        0,255,0,
        0,255,0,
        0,255,0,
        0,255,0
    };

    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };


    glm::vec2 centerOffSet[4] =
    {
        glm::vec2(-0.5f, +0.5f), glm::vec2(+0.5, +0.5),
        glm::vec2(+0.5f, -0.5f), glm::vec2(-0.5, -0.5)
    };

    //Fill with indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Fill with vertices
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Fill with colors
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Fill OffSet Buffer - Instancing Buffer
    glBindBuffer(GL_ARRAY_BUFFER, centerOffSetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(centerOffSet), centerOffSet, GL_STATIC_DRAW);




    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, centerOffSetVBO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribDivisor(2, 1);

    }
    glBindVertexArray(0);

}

int main()
{
    initOpenGL();

    Shader shader("Main.vert", "Main.frag");

    shader.CreateShaders();

    CreateQuad();

    // Cube cube;
   //  cube.Create();
    // cube.SetPosition(0.f, 0.f, -5.f);

     //glm::mat4 projection = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 10.f);

    shader.Bind();
    //shader.SendUniformData("projection", projection);

    /*glm::mat4 positionsMatrix[4] =
    {
        glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, +0.5f, 0.f)),
        glm::translate(glm::mat4(1.f), glm::vec3(+0.5f, +0.5f, 0.f)),
        glm::translate(glm::mat4(1.f), glm::vec3(+0.5f, -0.5f, 0.f)),
        glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, 0.f))
    };*/

    glm::mat4 resizeMatrix = glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.25f, 1.f));

    float startTime = glfwGetTime();

    glm::mat4 model[4];

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model[0] = glm::rotate(glm::mat4(1.f), -1.75f * dt, glm::vec3(0.f, 0.f, 1.f));
        /*model[1] = glm::rotate(glm::mat4(1.f), +0.75f * dt, glm::vec3(0.f, 0.f, 1.f));
        model[2] = glm::rotate(glm::mat4(1.f), -2.50f * dt, glm::vec3(0.f, 0.f, 1.f));
        model[3] = glm::rotate(glm::mat4(1.f), +1.25f * dt, glm::vec3(0.f, 0.f, 1.f));
        */

        //   cube.Draw();

        glBindVertexArray(VAO);
        {
            //Quad 1
            {
                glm::mat4 modelToWorld = resizeMatrix * model[0];
                shader.SendUniformData("Transformation", modelToWorld);
                glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);
            }


        }
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}