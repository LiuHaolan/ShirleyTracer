#include "WhittedIntegrator.h"
#include "..\World.h"
#include "..\materials.h"
vec3 WhittedIntegrator::Li(const ray& r, int depth) {

	if (depth > world->max_depth)
		return black;
	else {
		hit_record sr;
		//
//		sr.depth = depth;
		if (world->hit(r, 0.001, MAXFLOAT, sr)) {
			sr.r = r;
			sr.w = world;

			sr.depth = depth;
			//// this one is special
			sr.local_hit_point = sr.r.point_at_parameter(sr.t);
			sr.p = r.point_at_parameter(sr.t);


			return ((sr.mat_ptr)->shade(sr));
			//		return vec3(1.0, 1.0, 1.0);
		}
		else {
			// return the background color
			return world->background_color;
		}
	}
}