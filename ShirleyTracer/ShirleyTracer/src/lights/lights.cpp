#include "Ambient.h"
#include "PointLight.h"

vec3 Point_Light::get_direction(ShadeRec& sr) const {

	return (location - sr.p);

}

vec3 Point_Light::L(ShadeRec& sr) const {
	return (ls*color);
}

vec3 Ambient_Light::get_direction(ShadeRec& sr) const {
	// won't get anything to invoke this method.
	return vec3(0, 0, 0);

}

vec3 Ambient_Light::L(ShadeRec& sr) const {
	return (ls*color);
}

