#include "Emissive.h"

vec3 Emissive::area_light_shade(ShadeRec& sr) {
	if (-dot(sr.normal , sr.r.B) > 0.0)
		return (ls * ce);
	else
		return (vec3(0,0,0));
}
