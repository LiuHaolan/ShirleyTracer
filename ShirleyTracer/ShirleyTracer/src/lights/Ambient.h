#pragma once
#include "..\light.h"

class Ambient_Light : public Light {
public:

	Ambient_Light():ls(1.0),color(vec3(1.0,1.0,1.0)){}
	Ambient_Light(float ls_, vec3 color_):ls(ls_),color(color_){}
	vec3 get_direction(const hit_record& sr) const;

	bool in_shadows(const ray& r, const hit_record& sr) const { return false; }
	vec3 L(hit_record& sr) const;

private:
	float ls;
	vec3 color;

};