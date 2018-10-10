#pragma once
#include "..\light.h"

class PointLight : public Light {
public:

	PointLight() :ls(1.0), color(vec3(1.0, 1.0, 1.0)) { shadows = false; }
	PointLight(float ls_, vec3 color_,vec3 location_) :ls(ls_), color(color_), location(location_) { shadows = false; }
	vec3 get_direction(const hit_record& sr)const;

	vec3 L(hit_record& sr) const;
	bool in_shadows(const ray& r, const hit_record& sr) const;

private:
	float ls;
	vec3 color;
	vec3 location;
};
