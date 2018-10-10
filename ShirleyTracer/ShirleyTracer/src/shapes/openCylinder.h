#pragma once
#include "..\hitable.h"


class OpenCylinder : public hitable {
public:
	OpenCylinder(double bottom_, double top_, double r_) :y_bottom(bottom_), y_top(top_), radius(r_), y_norm(vec3(0, 1, 0)) {}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

private:
	double y_bottom;
	double y_top;
	double radius;
	vec3 y_norm;

};