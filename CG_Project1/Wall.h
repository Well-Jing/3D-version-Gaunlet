#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"

class Wall
{
public:
	Wall(Model _model, glm::vec3 _position, glm::vec3 _scale);

	Model model;
	glm::vec3 position;
	glm::vec3 scale;
	float x_length, z_length;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

};

