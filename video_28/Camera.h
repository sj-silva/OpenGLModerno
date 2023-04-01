#pragma once
#include<glm/glm.hpp>

class Camera
{
	glm::vec3 _position;
	glm::vec3 _viewDir;
	glm::vec3 _left;
	glm::vec3 _veloc;

	const glm::vec3 _UP = glm::vec3(0.f, 1.f, 0.f);
	const float _SPEED_MAG;

	glm::vec2 _oldMousePos;
	bool _isMouseActivated;

public:
	Camera();
	void mouseUpdate(const glm::vec2 currPos);
	glm::mat4 ativar();
	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
};

