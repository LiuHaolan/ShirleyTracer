#pragma once

#include "..\hitable.h"
#include "..\lanlog.h"

class Triangle :public hitable{
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

};
