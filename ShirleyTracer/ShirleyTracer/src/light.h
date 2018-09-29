#pragma once
#include "hitable.h"

class Light {
public:
	virtual vec3 get_direction(hit_record& sr) const = 0;
	virtual vec3 L(hit_record& sr) const = 0;

protected:
	bool shadows;
};