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
#include"Cube.h"

GLFWwindow* window;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void initOpenGL()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        fatalError("GLFW could not be initialized");
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Cube", NULL, NULL);
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


int main()
{
    

    initOpenGL();

    Shader shader("Main.vert", "Main.frag");

    shader.CreateShaders();

    Cube cube;
    cube.Create();
    cube.SetPosition(0.f, 0.f, -5.f);

    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 500.f);


    shader.Bind();
    shader.SendUniformData("projection", projection);

    
    float startTime = glfwGetTime();
    glm::mat4 model[4] = {};

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model[0] = glm::rotate(glm::mat4(1.f), -1.75f * dt, glm::vec3(1.f, 0.f, 0.f));
        model[0] = glm::rotate(model[0], +0.75f * dt, glm::vec3(0.f, 1.f, 0.f));
        model[0] = glm::rotate(model[0], -2.75f * dt, glm::vec3(0.f, 0.f, 1.f));


        model[1] = glm::rotate(glm::mat4(1.f), +0.75f * dt, glm::vec3(1.f, 0.f, 0.f));
        model[1] = glm::rotate(model[1], -3.75f * dt, glm::vec3(0.f, 1.f, 0.f));
        model[1] = glm::rotate(model[1], +1.75f * dt, glm::vec3(0.f, 0.f, 1.f));

        model[2] = glm::rotate(glm::mat4(1.f), +1.5f * dt, glm::vec3(1.f, 0.f, 0.f));
        model[2] = glm::rotate(model[2], +0.5f * dt, glm::vec3(0.f, 1.f, 0.f));
        model[2] = glm::rotate(model[2], +2.35f * dt, glm::vec3(0.f, 0.f, 1.f));

        model[3] = glm::rotate(glm::mat4(1.f), -1.75f * dt, glm::vec3(1.f, 0.f, 0.f));
        model[3] = glm::rotate(model[3], +0.75f * dt, glm::vec3(0.f, 1.f, 0.f));
        model[3] = glm::rotate(model[3], -3.75f * dt, glm::vec3(0.f, 0.f, 1.f));

        //Cube 1
        {
            cube.SetPosition(-2.5f, -2.5f, -15.f);
            glm::mat4 modelToWorld = cube.GetPosition() * model[0];
            shader.SendUniformData("model", modelToWorld);

            cube.Draw();
        }

        //Cube 2
        {
            cube.SetPosition(+2.5f, -2.5f, -15.f);


            glm::mat4 modelToWorld = cube.GetPosition() * model[1];
            shader.SendUniformData("model", modelToWorld);

            cube.Draw();
        }

        //Cube 3
        {
            cube.SetPosition(-5.5f, 5.f, -20.f);
            glm::mat4 modelToWorld = cube.GetPosition() * model[2];
            shader.SendUniformData("model", modelToWorld);

            cube.Draw();
        }

        //Cube 4
        {
            cube.SetPosition(+5.5f, 5.f, -20.f);
            glm::mat4 modelToWorld = cube.GetPosition() * model[3];
            shader.SendUniformData("model", modelToWorld);

            cube.Draw();
        }


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}