#include "RayCastIntegrator.h"
#include "..\utility.h"
#include "..\hitable.h"
#include "..\World.h"
#include "..\materials.h"

vec3 Integrator::Li(const ray& r, int depth) {
	return vec3(0, 0, 0);
}

vec3 RayCastIntegrator::Li(const ray& r, int depth) {
	hit_record sr;
	//
	if (world->hit(r, 0.001, MAXFLOAT, sr)) {
		sr.r = r;
		sr.w = world;

		//// this one is special
		//sr.local_hit_point = sr.r.point_at_parameter(sr.t);
		
		return ((sr.mat_ptr)->shade(sr));
//		return vec3(1.0, 1.0, 1.0);
	}
	else {
		// return the background color
		return world->background_color;
	}
}
