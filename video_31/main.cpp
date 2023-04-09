#include<gl/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<random>

#include"Error.h"
#include"Shader.h"
#include"Primitives.h"
#include"Camera.h"

GLFWwindow* window;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const unsigned int TOTAL_CUBES = 1000;
const unsigned int TOTAL_PYRAMIDS = 1000;

Camera camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

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

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

float  genRandom(float a, float b)
{
    std::random_device rd;
    int seed = rd();
    std::mt19937 rd_gen(seed);
    std::uniform_real_distribution<float> u_dist(a, b);

    return u_dist(rd_gen);
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


    Terrain terrain(250.f);
    terrain.Create();

    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 500.f);
   
    mainShader.Bind();
    mainShader.SendUniformData("projection", projection);
    mainShader.Unbind();

    minorShader.Bind();
    minorShader.SendUniformData("projection", projection);
    minorShader.SendUniformData("model", glm::mat4(1.f));
    minorShader.Unbind();

    // Initialize Position Matrix
    glm::mat4* cubesPosMatrix = new glm::mat4[TOTAL_CUBES];
    glm::mat4* pyramidsPosMatrix = new glm::mat4[TOTAL_PYRAMIDS];
    
    // Generate Objects Positions
    {
        // Cubes Positions
        for (int i = 0; i < TOTAL_CUBES; i++)
        {
            float x = genRandom(-200.f, 200.f);
            float y = genRandom(3.f, 10.f);
            float z = genRandom(-200.f, 200.f);
            cubesPosMatrix[i] = glm::translate(glm::vec3(x, y, z));
        }

        //Pyramids Positions
        for (int i = 0; i < TOTAL_PYRAMIDS; i++)
        {
            float x = genRandom(-200.f, 200.f);
            float y = genRandom(3.f, 15.f);
            float z = genRandom(-200.f, 200.f);
            pyramidsPosMatrix[i] = glm::translate(glm::vec3(x, y, z));
        }
    }

    // Initialize Angular Speed - Axis X, Y and Z
    glm::vec3* cubeAngSpeed = new glm::vec3[TOTAL_CUBES];
    glm::vec3* pyramidAngSpeed = new glm::vec3[TOTAL_PYRAMIDS];

    // Angular Speed
    {
        //Cube
        for (int i = 0; i < TOTAL_CUBES; i++)
        {
            float x = genRandom(-2.75f, -1.75f);
            float y = genRandom(1.f, 3.f);
            float z = genRandom(+1.f, 2.75f);
            cubeAngSpeed[i] = glm::vec3(x, y, z);
        }

        //Pyramid
        for (int i = 0; i < TOTAL_PYRAMIDS; i++)
        {
            float x = genRandom(+1.f, 2.75f);
            float y = genRandom(1.5f, 3.25f);
            float z = genRandom(-2.5f, -1.5f);

            pyramidAngSpeed[i] = glm::vec3(x, y, z);
        }
    }


    // Initialize Model Matrix
    glm::mat4* cubeModels = new glm::mat4[TOTAL_CUBES];
    glm::mat4* pyramidModels = new glm::mat4[TOTAL_PYRAMIDS];

    unsigned int cubeModelVBO = cube.GetModelVBO();
    unsigned int pyramidModelVBO = pyramid.GetModelVBO();

    float startTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        float currentTime = (float)glfwGetTime();
        float dt = currentTime - startTime;

        glm::mat4 viewMatrix = camera.ativar();



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Terrain Render
        {

            minorShader.Bind();
            minorShader.SendUniformData("view", viewMatrix);
            terrain.Draw();
            minorShader.Unbind();
        }

        mainShader.Bind();
        {
            mainShader.SendUniformData("view", viewMatrix);
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
                glBufferSubData(GL_ARRAY_BUFFER, 0, TOTAL_CUBES * sizeof(glm::mat4), cubeModels);

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
                glBufferSubData(GL_ARRAY_BUFFER, 0, TOTAL_PYRAMIDS * sizeof(glm::mat4), pyramidModels);

                pyramid.Draw();
            }

        }

        mainShader.Unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete[] cubeModels;
    delete[] cubesPosMatrix;
    delete[] cubeAngSpeed;

    delete[] pyramidModels;
    delete[] pyramidsPosMatrix;
    delete[] pyramidAngSpeed;

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Forward
        camera.moveForward();
    }
    else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Back
        camera.moveBack();

    }
    else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Left
        camera.moveLeft();

    }
    else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        // Right
        camera.moveRight();
    }

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouseUpdate(glm::vec2(xpos, ypos));
}