#include "Plane.h"
#include "..\utility.h"

static float kEpsilon = 0.001;

bool Plane::hitP(const ray& r, float & t) const {
	t = (dot(a, n) - dot(r.A, n)) / (dot(r.B, n));
	if (t > kEpsilon && t < MAXFLOAT) {

		return true;
	}

	return false;
}

bool Plane::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {

	float t = (dot(a, n) - dot(r.A, n)) / (dot(r.B, n));
	if (t > tmin && t < tmax) {
		rec.mat_ptr = mat;
		if (dot(n, r.B) < 0) {
			rec.normal = n;
		}
		else
			rec.normal = -n;
		rec.t = t;
		rec.p = r.point_at_parameter(t);

		return true;
	}

	return false;
}