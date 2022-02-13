#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) 
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}


Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;

	Forward.x = glm::cos(Pitch) * glm::cos(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::sin(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, WorldUp);
}

// the following three functions are about process input from mouse
void Camera::ProcessMouseMovement(float Delta_x, float Delta_y)
{
	Pitch += -Delta_y * SenseY;
	Yaw += Delta_x * SenseX;
	UpdateCameraVector();
}

void Camera::UpdateCameraVector() 
{
	Forward.x = glm::cos(Pitch) * glm::cos(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::sin(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::UpdateCameraPos()
{
	Position += Forward * SpeedZ * SenseSpeedZ + Right * SpeedX * SenseSpeedX + Up * SpeedY * SenseSpeedY;
}

// the rest are different camera mode
void Camera::ThirdPersonView_LookAtCharacter(Character character)
{
	//Position = character.position + glm::vec3(0.0f, 6.0f, 12.0f);
	Forward = glm::normalize((character.position + glm::vec3(0.0f, 2.0f, 0.0f))- Position);
	UpdateCameraVector_ThirdPersonView(character);
}

void Camera::UpdateCameraVector_ThirdPersonView(Character character)
{
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
	Position = (character.position + glm::vec3(0.0f, 2.0f, 0.0f)) - fov * Forward;
}

void Camera::UpdateCameraPos_ThirdPersonView(glm::vec3 displacement)
{
	Position += displacement;
}

void Camera::ProcessMouseMovement_ThirdPersonView(float Delta_x, float Delta_y)
{
	glm::vec3 temp_pos = Position + Delta_y * Up * 0.1f;
	if (temp_pos.y >= 0.9 * fov)
	{
		Position.y = 0.9 * fov;
	}
	else if (temp_pos.y <= 1.0f)
	{
		Position.y = 1.0f;
	}
	else
		Position += Delta_y * Up * 0.1f;
	
	Position += -Delta_x * Right * 0.1f;
}

void Camera::TopDownView_LookAtCharacter(Character character)
{
	Forward = glm::normalize((character.position + glm::vec3(0.0f, 2.0f, 0.0f)) - Position);
	UpdateCameraVector_TopDownView(character);
}

void Camera::UpdateCameraVector_TopDownView(Character character)
{
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));
	Position = (character.position + glm::vec3(0.0f, 2.0f, 0.0f)) - 30.0f * Forward;
}