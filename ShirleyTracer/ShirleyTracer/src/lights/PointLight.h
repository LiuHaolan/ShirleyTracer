#pragma once
#include "..\light.h"

class Point_Light : public Light {
public:

	Point_Light() :ls(1.0), color(vec3(1.0, 1.0, 1.0)) {}
	Point_Light(float ls_, vec3 color_,vec3 location_) :ls(ls_), color(color_), location(location_) {}
	virtual vec3 get_direction(ShadeRec& sr) const = 0;

	virtual vec3 L(ShadeRec& sr) const = 0;

private:
	float ls;
	vec3 color;
	vec3 location;
};
