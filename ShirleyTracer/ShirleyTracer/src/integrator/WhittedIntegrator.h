#pragma once
#include "..\integrator.h"

class WhittedIntegrator : public Integrator {
public:
	WhittedIntegrator(World* ptr) :Integrator(ptr) {}
	vec3 Li(const ray& r, int depth);


};