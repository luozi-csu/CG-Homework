#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _worldUp)
{
	position = _position;
	worldUp = _worldUp;
	forward = glm::normalize(_target - position);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

Camera::Camera(glm::vec3 _position, float _pitch, float _yaw, glm::vec3 _worldUp)
{
	position = _position;
	worldUp = _worldUp;
	pitch = _pitch;
	worldUp = _worldUp;
	forward.x = -glm::cos(pitch) * glm::sin(yaw);
	forward.y = -glm::sin(pitch);
	forward.z = -glm::cos(pitch) * glm::cos(yaw);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

glm::mat4 Camera::GetLookAtMatrix()
{
	return glm::lookAt(position, position + forward, worldUp);
}

void Camera::processMousemovement(float deltaX, float deltaY)
{
	pitch += deltaY * 0.005f;
	yaw -= deltaX * 0.005f;
	updateCameraVectors();
}

void Camera::updateCameraPosition()
{
	position -= forward * speedZ + right * speedX;
}

void Camera::updateCameraVectors()
{
	forward.x = -glm::cos(pitch) * glm::sin(yaw);
	forward.y = -glm::sin(pitch);
	forward.z = -glm::cos(pitch) * glm::cos(yaw);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}
