#include "GlossyReflective.h"
const static float kEpsilon = 0.0001;

vec3 GlossyReflective::area_light_shade(hit_record& sr) {

	// direct lighting
	vec3 L;
	L += Phong::area_light_shade(sr);

	vec3 wo = -sr.r.B;
	vec3 wi;
	float pdf;
	vec3 fr = glossy_specular_brdf->sample_f(sr, wo, wi, pdf);
	wi.make_unit_vector();
	ray reflected_ray(sr.p+wi*kEpsilon, wi);
	vec3 radiance = fr* sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1)*dot(unit_vector(sr.normal),unit_vector(wi)) / pdf;

	if (radiance.x() > 0) {
		int liyi = 1;
	}
	L += radiance;
	return L;
}