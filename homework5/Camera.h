#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float pitch;
	float yaw;
	float speedZ = 0.0f;
	float speedX = 0.0f;

	Camera(glm::vec3 _position, glm::vec3 _target, glm::vec3 _worldUp);
	Camera(glm::vec3 _position, float _pitch, float _yaw, glm::vec3 _worldUp);
	glm::mat4 GetLookAtMatrix();
	void processMousemovement(float deltaX, float deltaY);
	void updateCameraPosition();

private:
	void updateCameraVectors();
};

