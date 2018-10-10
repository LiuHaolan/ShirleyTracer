#include "Ambient.h"
#include "PointLight.h"
#include "DirectionLight.h"
#include "..\World.h"

bool PointLight::in_shadows(const ray& r, const hit_record& sr) const {
	float t;
	int num_objects = (sr.w)->objects.size();
	float d = (location - r.A).length();

	for (int j = 0; j < num_objects; j++) {
		if (sr.w->objects[j]->hitP(r, t) && t < d) {
			return true;
		}
	}

	return false;

}


vec3 PointLight::get_direction(const hit_record& sr) const {

	return (location - sr.p);

}

vec3 PointLight::L(hit_record& sr) const {
	return (ls*color);
}

vec3 Ambient_Light::get_direction(const hit_record& sr) const {
	// won't get anything to invoke this method.
	return vec3(0, 0, 0);

}

vec3 Ambient_Light::L(hit_record& sr) const {
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

vec3 DirectionLight::L(hit_record& s) const{
	return (ls * color);
}

vec3 DirectionLight::get_direction(const hit_record& sr) const {
	return dir;
}

bool DirectionLight::in_shadows(const ray& r, const hit_record& sr) const {

//	hit_record tmp;
	ray shadowray(r.A, get_direction(sr));
	int object_nums = sr.w->objects.size();

	for (int j = 0; j < object_nums; j++) {
		float t;

		if (sr.w->objects[j]->hitP(shadowray, t)) {
			return true;
		}
	}
	return false;
}