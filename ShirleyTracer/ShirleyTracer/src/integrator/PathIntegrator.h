#pragma once
#include "..\integrator.h"

class PathIntegrator : public Integrator {
public:
	PathIntegrator(World* ptr) :Integrator(ptr) {}
	vec3 Li(const ray& r, int depth);

};