#include "Disk.h"
const static float kEpsilon = 0.0001;
bool Disk::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	float t = (dot(a, normal) - dot(r.A, normal)) / (dot(r.B, normal));
	if (t > t_min && t < t_max) {
		rec.p = r.point_at_parameter(t);
		if ((rec.p - a).squared_length() < radius*radius) {
			rec.mat_ptr = mat;
			if (dot(normal, r.B) < 0) {
				rec.normal = normal;
			}
			else
				rec.normal = -normal;
			rec.t = t;
//			rec.p = r.point_at_parameter(t);
			rec.local_hit_point = rec.p;

			return true;
		}
	}

	return false;
}

bool Disk::hitP(const ray& r, float& t) const {

	t = (dot(a, normal) - dot(r.A, normal)) / (dot(r.B, normal));
	if (t > kEpsilon && t < MAXFLOAT) {
		vec3 p = r.point_at_parameter(t);
		if ((p - a).squared_length() < radius*radius) {
			return true;
		}
	}

	return false;
}