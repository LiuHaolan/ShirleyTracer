#pragma once

#include "..\integrator.h"  



class RayCastIntegrator : public Integrator {
public:
	RayCastIntegrator(World* ptr):Integrator(ptr){}
	vec3 Li(const ray& r, int depth);
};