#include "Ambient.h"
#include "PointLight.h"

vec3 Point_Light::get_direction(hit_record& sr) const {

	return (location - sr.p);

}

vec3 Point_Light::L(hit_record& sr) const {
	return (ls*color);
}

vec3 Ambient_Light::get_direction(hit_record& sr) const {
	// won't get anything to invoke this method.
	return vec3(0, 0, 0);

}

vec3 Ambient_Light::L(hit_record& sr) const {
	return (ls*color);
}

