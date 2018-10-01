#pragma once
#include "shaderec.h"

class Light {
public:
	virtual vec3 get_direction(ShadeRec& sr) const = 0;
	virtual vec3 L(ShadeRec& sr) const = 0;

protected:
	bool shadows;
};