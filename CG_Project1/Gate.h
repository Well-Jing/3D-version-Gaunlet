#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"

class Gate
{
public:
	Gate(Model _model, glm::vec3 _position, glm::vec3 _scale) :
		model(_model),
		position(_position),
		scale(_scale)
	{
	}

	Model model;
	glm::vec3 position;
	glm::vec3 scale;
	float x_length = 12.5;
	float z_length = 0.8;
};

