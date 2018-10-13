#include "Rectangle.h"

const float kEpsilon = 0.0001;

bool Rectangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	if (dot(normal, r.B) > 0)
		return false;
	
	double t = (dot(p0, normal) - dot(r.A, normal)) / (dot(r.B, normal));

	if (t < t_min || t>t_max)
		return (false);

	vec3 p = r.A + t * r.B;
	vec3 d = p - p0;

	double ddota = dot(d,a);

	if (ddota < 0.0 || ddota > a_len_squared)
		return (false);

	double ddotb = dot(d,b);

	if (ddotb < 0.0 || ddotb > b_len_squared)
		return (false);

	rec.t = t;
	rec.normal = normal;
	rec.mat_ptr = mat;
	rec.p = p;
	rec.local_hit_point = p;

	return (true);
}

bool Rectangle::hitP(const ray& r, float& t) const {
	t = dot((p0 - r.A), normal) / (dot(r.B, normal));

	if (t <= kEpsilon)
		return (false);

	vec3 p = r.A + t * r.B;
	vec3 d = p - p0;

	double ddota = dot(d, a);

	if (ddota < 0.0 || ddota > a_len_squared)
		return (false);

	double ddotb = dot(d, b);

	if (ddotb < 0.0 || ddotb > b_len_squared)
		return (false);

	return true;
}

void Rectangle::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
}

vec3 Rectangle::sample(void) {
	vec2 sample_point = sampler_ptr->sample_unit_square();
	return(p0 + sample_point.x()*a + sample_point.y()*b);
}

vec3 Rectangle::get_normal(const vec3& p) {
	return normal;
}

float Rectangle::pdf(const hit_record& sr) {
	return inv_area;
}