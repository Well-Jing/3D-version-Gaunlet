#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Model.h"


class IcePiton
{
public:
	IcePiton(Model _model, glm::vec3 _position, glm::vec3 _forward);

	Model model;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float radius = 0.5;
	float speed = 0.3;

	void updatePosition();

};

