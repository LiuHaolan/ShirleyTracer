#pragma once
#include "..\integrator.h"

class AreaLightIntegrator : public Integrator {
	vec3 Li(const ray& r, int depth);

};