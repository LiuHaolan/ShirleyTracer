#pragma once
#include "..\integrator.h"

class GlobalPathIntegrator : public Integrator {
public:
	GlobalPathIntegrator(World* ptr) :Integrator(ptr) {}
	vec3 Li(const ray& r, int depth);

};