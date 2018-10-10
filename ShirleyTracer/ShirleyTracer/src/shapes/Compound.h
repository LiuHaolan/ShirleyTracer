#pragma once

#include "..\hitable.h"
#include <vector>
using namespace std;

class Compound : public hitable {
public:
	void set_material(Material* mat_) {
		for (auto it = objects.cbegin(); it != objects.cend(); it++) {
			(*it)->set_material(mat_);
		}
	}

	void add_objects(hitable* object_ptr) {
		objects.push_back(object_ptr);
	}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

protected:
	vector<hitable*> objects;
};