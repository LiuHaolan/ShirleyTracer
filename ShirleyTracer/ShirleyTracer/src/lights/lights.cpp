#include "Ambient.h"
#include "PointLight.h"
#include "DirectionLight.h"

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

void
DirectionLight::scale_radiance(const float b) {
	ls = b;
}

void
DirectionLight::set_color(const float c) {
	color = vec3(c,c,c);
}

void
DirectionLight::set_color(const vec3& c) {
	color = c;
}

void
DirectionLight::set_color(const float r, const float g, const float b) {
	color = vec3(r, g, b);
}

void
DirectionLight::set_direction(vec3 d) {
	dir = d;
	dir.make_unit_vector();
}

vec3 DirectionLight::L(ShadeRec& s) const{
	return (ls * color);
}

vec3 DirectionLight::get_direction(hit_record& sr) const {
	return dir;
}