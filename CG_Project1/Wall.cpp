#include "Wall.h"


Wall::Wall(Model _model, glm::vec3 _position, glm::vec3 _scale):
	model(_model),
	position(_position),
	scale(_scale)
{
	x_length = scale.x;
	z_length = scale.z;
}