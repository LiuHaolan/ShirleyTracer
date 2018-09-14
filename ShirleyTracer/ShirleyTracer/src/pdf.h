#pragma once

#include "vec3.h"
#include "utility.h"

inline vec3 random_cosine_direction() {
	float r1 = randd();
	float r2 = randd();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI*r1;
	float x = cos(phi) * 2 * sqrt(r2);
	float y = sin(phi) * 2 * sqrt(r2);
	return vec3(x, y,  z);
}

class pdf {
public:
	virtual float value(const vec3& direction) const = 0;
	virtual vec3 generate() const = 0;
};
