#pragma once
#include "..\hitable.h"

class Plane : public hitable {
public:
	Plane(vec3 a_, vec3 n_) :a(a_), n(n_) {}
	bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	bool hitP(const ray& r, float & t) const;

private:
	vec3 a;	// a point on the plane
	vec3 n; // a normal on the plane
//	Material* mat;
};
