#pragma once

#include "ray.h"
#include "aabb.h"
#include "lanlog.h"

class Material;
class World;

// you need to include material files here!

struct hit_record {
	float t;
	ray r;
	vec3 p;

	vec3 local_hit_point;
	vec3 normal;
	Material* mat_ptr;
	World* w;

	float u, v;
};

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	virtual bool hitP(const ray& r, float& t) const {
		//Log here!
		lanlog::log_error("hitP base function called!");

		return false;
	}

	virtual bool bounding_box(float t0, float t1, aabb& box) const { return false; }

	virtual float pdf_value(const vec3& o, const vec3& v) const { return 0.0; }
	virtual vec3 random(const vec3& o) const { return vec3(1, 0, 0); }
	void set_material(Material* mat_) { mat = mat_; }
	Material* get_material() { return mat; }

protected:
	
	Material* mat;
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