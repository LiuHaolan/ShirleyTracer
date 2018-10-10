#include "Compound.h"

const static float kEpsilon = 0.0001; 

bool Compound::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	float t = MAXFLOAT;
	hit_record tmp;
	bool flag = false;

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->hit(r, t_min, t_max, tmp)) {
			if (tmp.t > t_min && tmp.t < t_max) {
				flag = true;

				if (tmp.t < t) {
					t = tmp.t;
					rec = tmp;
				}
			}
		}
	}

	return flag;
}

bool Compound::hitP(const ray& r, float& t) const {
	t = MAXFLOAT;
	float tmp;
	bool flag = false;

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->hitP(r, tmp)) {
			if (tmp > kEpsilon && tmp < MAXFLOAT) {
				flag = true;

				if (tmp < t) {
					t = tmp;
				}
			}
		}
	}

	return flag;
}