
#include "sphere.h"
#include "BBox.h"

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;

			rec.local_hit_point = r.point_at_parameter(rec.t);
			rec.local_hit_point /= radius;

			// consider hitting from inside or outside
			rec.topo_normal = rec.normal;
			if (dot(r.B, rec.normal) > 0.0)
				rec.normal = -rec.normal;

	//		get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;

			rec.local_hit_point = r.point_at_parameter(rec.t);
			rec.local_hit_point /= radius;

			rec.topo_normal = rec.normal;
					// consider hitting from inside or outside
			if (dot(r.B, rec.normal) > 0.0)
				rec.normal = -rec.normal;
	
			//		get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
	}
	return false;
}

bool sphere::hitP(const ray& r, float& t) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < MAXFLOAT && temp > kEpsilon) {
			t = temp;

			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < MAXFLOAT && temp > kEpsilon) {
			t = temp;
			return true;
		}
	}
	return false;

}

BBox sphere::get_bounding_box() {
	BBox box = BBox(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return box;
}