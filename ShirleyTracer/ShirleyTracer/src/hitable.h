#pragma once

#include "ray.h"
#include "aabb.h"
#include "lanlog.h"

// you need to include material files here!
class material;

struct hit_record {
	float t;
	vec3 p;
	vec3 normal;
	material* mat_ptr;
	float u, v;
};

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool hitP(const ray& r, float t_min, float t_max, hit_record& rec) const {
		//Log here!
		lanlog::log_error("hitP base function called!");

		return true;
	}

	virtual bool bounding_box(float t0, float t1, aabb& box) const { return false; }

	virtual float pdf_value(const vec3& o, const vec3& v) const { return 0.0; }
	virtual vec3 random(const vec3& o) const { return vec3(1, 0, 0); }
};


class flip_normals : public hitable {
public:
	flip_normals(hitable* p):ptr(p){}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
private:
	hitable * ptr;
};