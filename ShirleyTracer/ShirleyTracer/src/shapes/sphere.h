#pragma once
#include "..\hitable.h"
#include "utility.h"

static float kEpsilon = 0.0001;

class sphere : public hitable {
public:
	sphere():center(vec3(0,0,0)),radius(1) {};
	sphere(vec3 cen, float r) : center(cen), radius(r) { };
	sphere(vec3 cen, float r, Material* m) : center(cen), radius(r) { mat = m; };
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	bool hit(const ray& r, float& t) const;

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

			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat;
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
	}
	return false;
}

bool sphere::hit(const ray& r, float& t) const {
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