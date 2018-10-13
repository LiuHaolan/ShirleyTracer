#include "PathIntegrator.h"
#include "..\World.h"
#include "..\materials.h"

vec3 PathIntegrator::Li(const ray& r, int depth) {
	hit_record sr;

	if (depth > world->max_depth)
		return black;
	else {
		if (world->hit(r, 0.001, MAXFLOAT, sr)) {
			sr.r = r;
			sr.w = world;
			sr.p = r.point_at_parameter(sr.t);
			//			sr.local_hit_point = r.point_at_parameter(sr.t);
			sr.depth = depth;

			return ((sr.mat_ptr)->path_shade(sr));
	//				return vec3(1.0, 1.0, 1.0);
		}
		else {
			// return the background color
			return world->background_color;
		}
	}
}