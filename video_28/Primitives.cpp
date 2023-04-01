#include<gl/glew.h>
#include<glm/gtx/transform.hpp>

#include "Primitives.h"


Primitive::Primitive() :
    _VAO(0), _EBO(0),
    _positionVBO(0), _colorVBO(0),
    _modelVBO(0), _total(1),
    _position(0.f), _model(1.f)
{
}

Primitive::~Primitive()
{
    glDeleteBuffers(1, &_positionVBO);
    glDeleteBuffers(1, &_colorVBO);
    glDeleteBuffers(1, &_modelVBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteVertexArrays(1, &_VAO);
}

void Primitive::Create()
{
    CreateBuffers();
    FillBuffers();
    LinkBuffers();
}

void Primitive::CreateBuffers()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_EBO);
    glGenBuffers(1, &_positionVBO);
    glGenBuffers(1, &_colorVBO);
    glGenBuffers(1, &_modelVBO);
}

void Primitive::UpdatePosition()
{
    _model = glm::translate(_position);
}

void Primitive::SetPosition(float x, float y, float z)
{
    _position = glm::vec3(x, y, z);
    UpdatePosition();
}

glm::mat4 Primitive::GetPosition() const
{
    return _model;
}

unsigned int Primitive::GetModelVBO() const
{
    return _modelVBO;
}

