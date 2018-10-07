#pragma once
#include "..\hitable.h"
#include "..\utility.h"

static float kEpsilon = 0.0001;

class sphere : public hitable {
public:
	sphere():center(vec3(0,0,0)),radius(1) {};
	sphere(vec3 cen, float r) : center(cen), radius(r) { };
	sphere(vec3 cen, float r, Material* m) : center(cen), radius(r) { mat = m; };
	bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

	virtual bool bounding_box(float t0, float t1, aabb& box) const { 
		box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
		return true;
	}

	//void set_material(Material* m) { mat = m; }
	float get_radius() const{ return radius; }
	vec3 get_center() const{ return center; }

private:
	vec3 center;
	float radius;

};

