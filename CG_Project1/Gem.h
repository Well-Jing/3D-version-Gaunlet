#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"

class Gem
{
public:
	Gem(Model _model, glm::vec3 _position) :
		model(_model),
		position(_position)
	{
	}

	Model model;
	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float radius = 1.0;
};

