#include "Emissive.h"

vec3 Emissive::area_light_shade(hit_record& sr) {
	if (-dot(sr.normal , sr.r.B) > 0.0)
		return (ls * ce);
	else
		return (black);
}

vec3 Emissive::path_shade(hit_record& sr) {
	if (-dot(sr.normal, sr.r.B) > 0.0)
		return (ls * ce);
	else
		return (black);
}

vec3 Emissive::global_path_shade(hit_record& sr) {
	if (sr.depth == 1) {
		return black;
	}
	else {
		if (-dot(sr.normal, sr.r.B) > 0.0)
			return (ls * ce);
		else
			return (black);
	}
}
