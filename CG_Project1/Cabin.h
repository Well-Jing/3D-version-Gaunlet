#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"

class Cabin
{
public:
	Cabin(Model _model, glm::vec3 _position, glm::vec3 _scale, float _rotation, glm::vec3 _rotation_dir) :
		model(_model),
		position(_position),
		scale(_scale),
		rotation(_rotation),
		rotation_dir(_rotation_dir)
	{
	}

	Model model;
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec3 rotation_dir;
};

