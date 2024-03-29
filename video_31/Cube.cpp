#include<gl/glew.h>
#include<glm/gtc/matrix_transform.hpp>

#include "Primitives.h"

Cube::Cube(unsigned int totalCubes)
{
    _total = totalCubes;
}

void Cube::FillBuffers()
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


    //Fill with indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Fill with vertices
    glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Fill with colors
    glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); 
    
    // Create empty model Buffer
    glBindBuffer(GL_ARRAY_BUFFER, _modelVBO);
    glBufferData(GL_ARRAY_BUFFER, _total*sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);

}

void Cube::LinkBuffers()
{
    glBindVertexArray(_VAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

        glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(GLubyte), 0);


        glBindBuffer(GL_ARRAY_BUFFER, _modelVBO);
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

void Cube::Draw()
{
    glBindVertexArray(_VAO);
    {
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, _total);
    }
    glBindVertexArray(0);

}


