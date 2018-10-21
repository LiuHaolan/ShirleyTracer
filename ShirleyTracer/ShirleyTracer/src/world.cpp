#include "World.h"
bool World::inside;

void World::add_light(Light* lit) {
	lights.push_back(lit);
}

void World::add_object(hitable* obj) {
	objects.push_back(obj);
}

bool World::hit(const ray& r, float tmin,float tmax, hit_record& sr) {

	bool flag = false;
	float t = MAXFLOAT;
	for (int j = 0; j < this->objects.size(); j++) {
		hitable* obj = objects[j];

		hit_record tmp;

		if (obj->hit(r, tmin, tmax, tmp)) {
			if (tmp.t < t) {
				
				// debug
				sr.name = obj->name;

				t = tmp.t;
				sr = tmp;
				flag = true;
			}
		}

	}

	return flag;
}