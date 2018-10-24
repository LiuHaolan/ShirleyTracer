#include "SVReflective.h"

vec3 SVReflective::shade(hit_record& sr) {
	vec3 L(SVPhong::shade(sr));

	vec3 wo = -sr.r.B;
	vec3 wi;
	vec3 fr = reflective_brdf->sample_f(sr, wo, wi);
	ray reflected_ray(sr.p + wi * 0.001, wi);

	L += fr * sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1) * (dot(sr.normal, wi) / sr.normal.length() / wi.length());

	return (L);
}