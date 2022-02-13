#include "IcePiton.h"

IcePiton::IcePiton(Model _model, glm::vec3 _position, glm::vec3 _forward):
	model(_model),
	position(_position),
	forward(_forward)
{
}



void IcePiton::updatePosition()
{
	position += forward * speed;
}


