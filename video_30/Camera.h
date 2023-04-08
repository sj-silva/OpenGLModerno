#pragma once
#include<glm/glm.hpp>

class Camera
{
	glm::vec3 _position;
	glm::vec3 _viewDir;
	glm::vec3 _left;

	const glm::vec3 _UP;
	const float _SPEED_MAG;

	// Mouse
	glm::vec2 _oldMousePos;
	bool _isMouseActivated;
	
	void setMag(glm::vec2& v, float val);

public:
	Camera();

	void mouseUpdate(const glm::vec2& currentPos);

	glm::mat4 ativar();
	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
};