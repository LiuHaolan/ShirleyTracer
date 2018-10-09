#pragma once
#include "..\integrator.h"

class AreaLightIntegrator : public Integrator {
public:
	AreaLightIntegrator(World* ptr) :Integrator(ptr) {}
	vec3 Li(const ray& r, int depth);

};