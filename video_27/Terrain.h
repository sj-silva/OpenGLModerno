#pragma once
#include<glm/glm.hpp>

class Terrain
{

public:
	Terrain(float side = 50.f);
	~Terrain();
	void Create();
	void Draw();

private:
	unsigned int _VAO;
	unsigned int _EBO;
	unsigned int _verticesVBO;
	unsigned int _colorVBO;
	float _side;
	unsigned int _totalIndices;

	void CreateBuffers();
	void FillBuffers();
	void LinkBuffers();


};

