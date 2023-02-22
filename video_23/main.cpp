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

GLFWwindow* window;
unsigned int VAO;

unsigned int matrixListVBO = 0;
glm::mat4 matrixList[4] = { glm::mat4(1.f)};

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
    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &positionVBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &matrixListVBO);

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

    
    //Fill with indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Fill with vertices
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Fill with colors
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Fill Matrix Buffer - Instancing Buffer
    glBindBuffer(GL_ARRAY_BUFFER, matrixListVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(matrixList), 0, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, matrixListVBO);
        {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 0));

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 1));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));

            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));

            glVertexAttribDivisor(2, 1);
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);

        }
        
    }
    glBindVertexArray(0);
}

int main()
{
    initOpenGL();

    Shader shader("Main.vert", "Main.frag");

    shader.CreateShaders();

    CreateQuad();

    shader.Bind();
    
    glm::mat4 positionsMatrix[4] =
    {
        glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, +0.5f, 0.f)),
        glm::translate(glm::mat4(1.f), glm::vec3(+0.5f, +0.5f, 0.f)),
        glm::translate(glm::mat4(1.f), glm::vec3(+0.5f, -0.5f, 0.f)),
        glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, 0.f))
    };

    glm::mat4 resizeMatrix = glm::scale(glm::mat4(1.f), glm::vec3(0.25f, 0.25f, 1.f));

    

    float startTime = glfwGetTime();

    glm::mat4 model[4] = { glm::mat4(1.f) };

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model[0] = glm::rotate(glm::mat4(1.f), -1.75f * dt, glm::vec3(0.f, 0.f, 1.f));
        model[1] = glm::rotate(glm::mat4(1.f), +0.75f * dt, glm::vec3(0.f, 0.f, 1.f));
        model[2] = glm::rotate(glm::mat4(1.f), -2.50f * dt, glm::vec3(0.f, 0.f, 1.f));
        model[3] = glm::rotate(glm::mat4(1.f), +1.25f * dt, glm::vec3(0.f, 0.f, 1.f));
        
  
        for (int i = 0; i < 4; i++)
        {
           matrixList[i] = positionsMatrix[i] * resizeMatrix * model[i];
        }

        glBindBuffer(GL_ARRAY_BUFFER, matrixListVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(matrixList), matrixList);

        glBindVertexArray(VAO);
        {
                glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4);   
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