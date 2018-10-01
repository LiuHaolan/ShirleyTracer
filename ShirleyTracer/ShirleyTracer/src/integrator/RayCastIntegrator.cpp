#include "RayCastIntegrator.h"
#include "..\utility.h"
#include "..\shaderec.h"
vec3 Integrator::Li(const ray& r, int depth) {
	return vec3(0, 0, 0);
}

vec3 RayCastIntegrator::Li(const ray& r, int depth) {
	//hit_record sr;
	//
	//if (world->hit(r, 0.001, MAXFLOAT, sr)) {

	//	
	//}
	//else {
	//	// return the background color
	//	return vec3(0, 0, 0);
	//}
	return vec3(0, 0, 0);
}
