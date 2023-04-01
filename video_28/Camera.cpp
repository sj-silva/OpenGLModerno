#include "Camera.h"

Camera::Camera():
	_position(glm::vec3(0.f)),
	_viewDir(glm::vec3(0.f, 0.f, -1.f)),
	_left(-1.f, 0.f, 0.f),
	_veloc(glm::vec3(0.f)),
	_UP(glm::vec3(0.f, 1.f, 0.f)),
	_SPEED_MAG(0.05f),
	_oldMousePos(glm::vec2(0.f)),
	_isMouseActivated(false)
{   
}                   

void Camera::mouseUpdate(const glm::vec2 currPos)
{
}

glm::mat4 Camera::ativar()
{
    return glm::mat4();
}

void Camera::moveForward()
{

}

void Camera::moveBack()
{

}

void Camera::moveLeft()
{

}

void Camera::moveRight()
{

}

void Camera::moveUp()
{

}

void Camera::moveDown()
{

}
