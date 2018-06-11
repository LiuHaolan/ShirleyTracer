#pragma once

#include "hitable.h"

class hitable_list : public hitable {
public:
	hitable_list() {};
	hitable_list(hitable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

	hitable **list;
	int list_size;
};

bool hitable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	
	float tnow = tmax;
	hit_record hit_now;
	bool hit_or_not = false;
	for (int k = 0; k < list_size; k++) {
		if (list[k]->hit(r, tmin, tnow, hit_now)) {
			hit_or_not = true;
			tnow = hit_now.t;
			rec = hit_now;
		}

	}
	return hit_or_not;
}