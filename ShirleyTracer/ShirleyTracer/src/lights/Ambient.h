#pragma once
#include "..\light.h"

class Ambient_Light : public Light {
public:

	Ambient_Light():ls(1.0),color(vec3(1.0,1.0,1.0)){}
	Ambient_Light(float ls_, vec3 color_):ls(ls_),color(color_){}
	vec3 get_direction(ShadeRec& sr) const;

	vec3 L(ShadeRec& sr) const;

private:
	float ls;
	vec3 color;

};