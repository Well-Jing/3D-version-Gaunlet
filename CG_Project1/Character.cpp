#include "Character.h"

Character::Character(Model _model, glm::vec3 _position, glm::vec3 _scale, float _speed):
	model(_model),
	position(_position),
	scale(_scale),
	speed(_speed)
{
}

Character::Character(ffModel _model, glm::vec3 _position, glm::vec3 _scale, float _speed) :
	ffmodel(_model),
	position(_position),
	scale(_scale),
	speed(_speed)
{
}

glm::vec3 Character::updatePosition_key()
{
	glm::vec3 displacement = senseForward * speedForward * forward * speed + senseRight * speedRight * right * speed;

	// if collide with walls
	if (collision_x_p && displacement.x < 0)
		displacement.x = 0;
	if (collision_x_n && displacement.x > 0)
		displacement.x = 0;
	if (collision_z_p && displacement.z < 0)
		displacement.z = 0;
	if (collision_z_n && displacement.z > 0)
		displacement.z = 0;

	position += displacement;

	glm::mat4 trans;
	trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
	oriantation =  trans * glm::vec4(initialDir, 1.0f);

	return displacement;
}

glm::vec3 Character::updatePosition_follow(glm::vec3 destination)
{
	glm::vec3 displacement;

	if (glm::length(destination - position) >= 0.3)
	{
		displacement = senseForward * forward * speed;

		if (collision_x_p && displacement.x < 0)
			displacement.x = 0;
		if (collision_x_n && displacement.x > 0)
			displacement.x = 0;
		if (collision_z_p && displacement.z < 0)
			displacement.z = 0;
		if (collision_z_n && displacement.z > 0)
			displacement.z = 0;

		position += displacement;
	}
	else
	{
		displacement = glm::vec3(0.0f);
	}

	return displacement;
}


void Character::updateDirection_camera(glm::vec3 _forward, glm::vec3 _right)
{
	forward = glm::normalize(glm::vec3(_forward.x, 0.0f, _forward.z));
	right = _right;
}

void Character::updateDirection_follow(glm::vec3 followDirection)
{
	forward = glm::normalize(followDirection);
	right = glm::normalize(cross(forward, worldUp));
}
