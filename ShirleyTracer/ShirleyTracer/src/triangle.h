#pragma once

#include "hitable.h"
#include "lanlog.h"

class Triangle :hitable{
public:
	Triangle(vec3 a, vec3 b, vec3 c) {
		vertex[0] = a;
		vertex[1] = b;
		vertex[2] = c;
	}

	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float t_min, float t_max, hit_record& rec) const;
private:
	vec3 vertex[3];
	material* mat;
};

bool Triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	lanlog::log_info("triangle hit function not implemented");
	return false;
}

bool Triangle::hitP(const ray& r, float t_min, float t_max, hit_record& rec) const {
	lanlog::log_info("triangle hitP function not implemented");
	return false;
}