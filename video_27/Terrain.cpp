#include<gl/glew.h>
#include<vector>

#include "Terrain.h"

Terrain::Terrain(float side):
	_VAO(0), _EBO(0),
	_verticesVBO(0), _colorVBO(0),
	_side(side), _totalIndices(0)
{
}

Terrain::~Terrain()
{
	glDeleteBuffers(1, &_verticesVBO);
	glDeleteBuffers(1, &_colorVBO);
	glDeleteBuffers(1, &_EBO);
	glDeleteVertexArrays(1, &_VAO);
}

void Terrain::Create()
{
	CreateBuffers();
	FillBuffers();
	LinkBuffers();
}

void Terrain::Draw()
{
	glBindVertexArray(_VAO);
	{
		 glDrawElements(GL_LINES, _totalIndices, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Terrain::CreateBuffers()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_verticesVBO);
	glGenBuffers(1, &_colorVBO);
	
}

void Terrain::FillBuffers()
{
   
	std::vector<glm::vec3> vertices;
    std::vector<GLubyte> colors;
	std::vector<unsigned int> indices;

    float y = -0.5f;
    float incr = 1.f;

    // Create Vertcies
    for (float z = -_side; z <= _side; z+=incr)// row
        for (float x = -_side; x <= _side; x+=incr)// col
        {
            vertices.push_back(glm::vec3(x, y, z));
           
            //Ice White
            colors.push_back(229);
            colors.push_back(229);
            colors.push_back(224);
        }
    // Create Indices
    unsigned int columnSize = 2 * static_cast<unsigned int>(_side) + 1;

    for (unsigned int row = 0; row < columnSize - 1; row++) 
	{
        for (unsigned int col = 0; col < columnSize - 1; col++) 
		{
            indices.push_back(columnSize * row + col);//upper left
            indices.push_back(columnSize * row + col + 1);//upper right

            indices.push_back(columnSize * row + col);//upper left
            indices.push_back(columnSize * row + col + columnSize);//bottom left
            
            indices.push_back(columnSize * row + col + columnSize + 1);//bottom right
            indices.push_back(columnSize * row + col + columnSize);//bottom left

            if (col == columnSize - 2)
            {
                indices.push_back(columnSize * row + col + 1);//upper right
                indices.push_back(columnSize * row + col + columnSize + 1);//bottom right
            }
        }
    }

    _totalIndices = indices.size();

    //Fill with indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Fill with vertices
    glBindBuffer(GL_ARRAY_BUFFER, _verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Fill with colors
    glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(GLubyte), colors.data(), GL_STATIC_DRAW);

}

void Terrain::LinkBuffers()
{
    glBindVertexArray(_VAO);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

        glBindBuffer(GL_ARRAY_BUFFER, _verticesVBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(GLubyte), 0);
    }
    glBindVertexArray(0);
}
