#include "AreaLightIntegrator.h"
#include "..\World.h"
#include "..\utility.h"
#include "..\materials.h"

vec3 AreaLightIntegrator::Li(const ray& r, int depth) {
	hit_record sr;
	//
	if (world->hit(r, 0.001, MAXFLOAT, sr)) {
		sr.r = r;
		sr.w = world;
		return ((sr.mat_ptr)->area_light_shade(sr));
		//		return vec3(1.0, 1.0, 1.0);
	}
	else {
		// return the background color
		return world->background_color;
	}
}

