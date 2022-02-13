#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Mesh.h"
#include "Model.h"
#include "ffMesh.h"
#include "ffModel.h"
//#include "Camera.h"

class Character
{
public:
	Character(Model _model, glm::vec3 _position, glm::vec3 _scale, float _speed);
	Character(ffModel _model, glm::vec3 _position, glm::vec3 _scale, float _speed);


	Model model;
	ffModel ffmodel;
	glm::vec3 initialDir = glm::vec3(0.0f, 0.0f, 1.0f);
	float rotate = 0;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 oriantation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float radius = 1.0;
	float speed;
	unsigned int numKey = 0;
	unsigned int numGem = 0;
	unsigned int numLife = 10;

	bool collision_x_p = false;
	bool collision_x_n = false;
	bool collision_z_p = false;
	bool collision_z_n = false;
	bool die = false;
	bool win = false;

	float senseForward = 0.15;
	float senseRight = 0.15;
	float senseUp = 0.15;
	float speedForward = 0;
	float speedRight = 0;
	float speedUp = 0;

	glm::vec3 updatePosition_key();
	glm::vec3 updatePosition_follow(glm::vec3 destination);
	void updateDirection_camera(glm::vec3 _forward, glm::vec3 _right);
	void updateDirection_follow(glm::vec3 followDirection);

};

