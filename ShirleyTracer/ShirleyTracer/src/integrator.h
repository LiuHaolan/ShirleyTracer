#pragma once

#include "hitable.h"
#include "ray.h"
//#include "World.h"

// forward reference again
class World;
class Integrator {
public:
	Integrator(World* ptr) : world(ptr) {}
	virtual vec3 Li(const ray& r, int depth);
protected:
	World* world;
};
