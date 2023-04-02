#include "Camera.h"
#include<glm/gtx/transform.hpp>

Camera::Camera():
	_position(glm::vec3(0.f)),
	_viewDir(glm::vec3(0.f, 0.f, -1.f)),
	_left(-1.f, 0.f, 0.f),
	_UP(glm::vec3(0.f, 1.f, 0.f)),
	_SPEED_MAG(0.05f)
{   
}                   



glm::mat4 Camera::ativar()
{
	return glm::lookAt(_position, _position + _viewDir, _UP);
}

void Camera::moveForward()
{
	_position = _position + _viewDir * _SPEED_MAG;
}

void Camera::moveBack()
{
	_position = _position + _viewDir * (-_SPEED_MAG);
}

void Camera::moveLeft()
{
	_position = _position + _left * _SPEED_MAG;
}

void Camera::moveRight()
{
	_position = _position + _left * (-_SPEED_MAG);
}

