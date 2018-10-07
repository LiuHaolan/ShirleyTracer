#pragma once

#include "vec3.h"
#include "hitable.h"

class Texture {
public:
	virtual vec3 value(const hit_record& sr) const = 0;
};
