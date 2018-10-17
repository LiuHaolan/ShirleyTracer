#include "Transparent.h"

vec3 Transparent::shade(hit_record& sr) {
	vec3 L(Phong::shade(sr));

	vec3 wo = unit_vector(-sr.r.B);
	vec3 wi(0,0,0);
	vec3 fr = reflective_brdf->sample_f(sr, wo, wi);
	ray reflected_ray(sr.p+0.001*wi, wi);

	if (specular_btdf->tir(sr)) {
		L += sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1);
	}
	else {
		vec3 wt;
		vec3 ft = specular_btdf->sample_f(sr, wo, wt);
		ray transmitted_ray(sr.p+wt*0.001, wt);

		L +=  fr * (sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1)) *fabs(dot(sr.normal, wi) / sr.normal.length()/wi.length());
		
		sr.w->inside = !sr.w->inside;
		L +=  ft * (sr.w->integrator_ptr->Li(transmitted_ray, sr.depth + 1)) *fabs(dot(sr.normal, wt) / sr.normal.length()/wt.length());
		sr.w->inside = !sr.w->inside;
	}

	return L;
}