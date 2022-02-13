#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"

class Key
{
public:
	Key(Model _model, glm::vec3 _position) :
		model(_model),
		position(_position)
	{
	}

	Model model;
	glm::vec3 position;
	glm::vec3 scale = glm::vec3(3.0f, 3.0f, 3.0f);
	float radius = 1.0;
};

