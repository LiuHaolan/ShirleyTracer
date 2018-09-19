#pragma once

#include "hitable.h"
#include "ray.h"


class Integrator {
public:
	virtual vec3 Li(const ray& r, hitable *world, int depth) = 0;

};
