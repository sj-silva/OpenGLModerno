#pragma once
#include<glm/glm.hpp>

class Cube
{
public:
	Cube(unsigned int nCubes);
	~Cube();

	void Create();

	void SetPosition(float x, float y, float z);
	

	glm::mat4 GetPosition() const;
	unsigned int GetModelVBO() const;
	

	void Draw();

private:
	unsigned int _VAO;
	unsigned int _EBO;
	unsigned int _positionVBO;
	unsigned int _colorVBO;
	unsigned int _modelVBO;

	unsigned int _totalCubes;

	glm::vec3 _position;
	glm::mat4 _model;

	void CreateBuffers();
	void FillBuffers();
	void LinkBuffers();

	void UpdatePosition();
};