#pragma once
#include<glm/glm.hpp>

class Camera
{
	glm::vec3 _position;
	glm::vec3 _viewDir;
	glm::vec3 _left;
	
	const glm::vec3 _UP;
	const float _SPEED_MAG;

public:
	Camera();
	
	glm::mat4 ativar();
	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
};

