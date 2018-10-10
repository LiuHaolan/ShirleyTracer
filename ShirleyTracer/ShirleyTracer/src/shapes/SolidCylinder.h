#pragma once
#include "../hitable.h"
#include "Compound.h"
#include "openCylinder.h"
#include "Disk.h"

class SolidCylinder : public Compound {
public:
	SolidCylinder(const float bottom, const float top, const float radius) {
		objects.push_back(new Disk(vec3(0, bottom, 0), vec3(0, -1, 0), radius));
		objects.push_back(new Disk(vec3(0, top, 0), vec3(0, 1, 0), radius));
		objects.push_back(new OpenCylinder(bottom, top, radius));
	}

	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;


};

