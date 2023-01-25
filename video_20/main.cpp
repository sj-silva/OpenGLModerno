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


int main()
{
    

    initOpenGL();

    Shader shader("Main.vert", "Main.frag");

    shader.CreateShaders();

    Cube cube;
    cube.Create();
    cube.SetPosition(0.f, 0.f, -5.f);

    glm::mat4 projection = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 10.f);

    shader.Bind();
    shader.SendUniformData("projection", projection);

    
    float startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = glfwGetTime();
        float dt = currentTime - startTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::rotate(glm::mat4(1.f), -dt, glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, 1.75f * dt, glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, 0.75f * dt, glm::vec3(0.f, 0.f, 1.f));
        
        glm::mat4 modelToWorld = cube.GetPosition() * model ;
        shader.SendUniformData("model", modelToWorld);

        cube.Draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}