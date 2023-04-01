#include<gl/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<string>
#include<fstream>
#include<sstream>

#include"Error.h"
#include"Shader.h"
#include"Primitives.h"


GLFWwindow* window;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const unsigned int TOTAL_CUBES = 4;
const unsigned int TOTAL_PYRAMIDS = 4;

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

    //Model Matrix  with Frequent Changes
    Shader mainShader("Main.vert", "Main.frag");
    mainShader.CreateShaders();

    //Model Matrix  with Minor Changes
    Shader minorShader("Minor.vert", "Main.frag");
    minorShader.CreateShaders();

    Cube cube(TOTAL_CUBES);
    cube.Create();

    Pyramid pyramid(TOTAL_PYRAMIDS);
    pyramid.Create();


    Terrain terrain;
    terrain.Create();
   
    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 500.f);
    //glm::mat4 minorModel = glm::mat4(1.f);

    mainShader.Bind();
    mainShader.SendUniformData("projection", projection);
    mainShader.Unbind();

    minorShader.Bind();
    minorShader.SendUniformData("projection", projection);
    minorShader.SendUniformData("model", glm::mat4(1.f));
    minorShader.Unbind();

    
    glm::mat4 cubeModels[TOTAL_CUBES] = {};
    glm::mat4 pyramidModels[TOTAL_PYRAMIDS] = {};

    // Position Matrix
    glm::mat4 cubesPosMatrix[TOTAL_CUBES] =
    {
      glm::translate(glm::vec3(-2.5f, 3.5f, -15.f)),
      glm::translate(glm::vec3(+2.5f, 3.5f, -15.f)),
      glm::translate(glm::vec3(-5.5f, 5.f, -20.f)),
      glm::translate(glm::vec3(+5.5f, 5.f, -20.f))
    };
    
    glm::mat4 pyramidsPosMatrix[TOTAL_PYRAMIDS] =
    {
      glm::translate(glm::vec3(-4.5f, 2.5f, -25.f)),
      glm::translate(glm::vec3(+2.5f, 2.5f, -25.f)),
      glm::translate(glm::vec3(-4.5f, 2.f, -10.f)),
      glm::translate(glm::vec3(+3.5f, 2.f, -15.f))
    };


    //Angular Speed - Axis X, Y and Z
    glm::vec3 cubeAngSpeed[TOTAL_CUBES] =
    {
        glm::vec3(-1.75f, +0.75f, -2.75f),
        glm::vec3(+0.75f, -3.75f, +1.75f),
        glm::vec3(+1.50f, +0.50f, +2.35f),
        glm::vec3(-1.75f, +0.75f, -3.75f)
    };
    
    glm::vec3 pyramidAngSpeed[TOTAL_PYRAMIDS] =
    {
        glm::vec3(-0.55f, -2.60f, +0.40f),
        glm::vec3(+0.50f, +1.75f, -0.42f),
        glm::vec3(+0.70f, +2.30f, -0.80f),
        glm::vec3(+0.80f, -3.52f, +0.52f)
    };

    unsigned int cubeModelVBO = cube.GetModelVBO();
    unsigned int pyramidModelVBO = pyramid.GetModelVBO();
    
    float startTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = (float)glfwGetTime();
        float dt = currentTime - startTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Terrain Render
        {
           
            minorShader.Bind();
            terrain.Draw();
            minorShader.Unbind();
        }
  
        mainShader.Bind();
        {
            //Cube 
            {
                for (unsigned int i = 0; i < TOTAL_CUBES; i++)
                {
                    cubeModels[i] = glm::rotate(cubeAngSpeed[i].x * dt, glm::vec3(1.f, 0.f, 0.f));
                    cubeModels[i] = glm::rotate(cubeModels[i], cubeAngSpeed[i].y * dt, glm::vec3(0.f, 1.f, 0.f));
                    cubeModels[i] = cubesPosMatrix[i] * glm::rotate(cubeModels[i], cubeAngSpeed[i].z * dt, glm::vec3(0.f, 0.f, 1.f));
                }

                // Update Matrix Buffer
                glBindBuffer(GL_ARRAY_BUFFER, cubeModelVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeModels), cubeModels);


                cube.Draw();
                
            }

            //Pyramid 
            {
                for (unsigned int i = 0; i < TOTAL_PYRAMIDS; i++)
                {
                    pyramidModels[i] = glm::rotate(pyramidAngSpeed[i].x * dt, glm::vec3(1.f, 0.f, 0.f));
                    pyramidModels[i] = glm::rotate(pyramidModels[i], pyramidAngSpeed[i].y * dt, glm::vec3(0.f, 1.f, 0.f));
                    pyramidModels[i] = pyramidsPosMatrix[i] * glm::rotate(pyramidModels[i], pyramidAngSpeed[i].z * dt, glm::vec3(0.f, 0.f, 1.f));
                }

                // Update Matrix Buffer
                glBindBuffer(GL_ARRAY_BUFFER, pyramidModelVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramidModels), pyramidModels);

                pyramid.Draw();
            }
            
        }

        mainShader.Unbind();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}