#pragma once
#include "..\light.h"

class Ambient_Light : public Light {
public:

	Ambient_Light():ls(1.0),color(vec3(1.0,1.0,1.0)){}
	Ambient_Light(float ls_, vec3 color_):ls(ls_),color(color_){}
	virtual vec3 get_direction(hit_record& sr) const = 0;

	virtual vec3 L(hit_record& sr) const = 0;

private:
	float ls;
	vec3 color;

};