
#include "SVPhong.h"

vec3 SVPhong::shade(hit_record& sr) {
	vec3 wo = -sr.r.B;
	//	assert(sr.w->ambient_ptr != 0);

	vec3 	L = ambient_brdf->rho(sr, wo) * sr.w->ambient_ptr->L(sr);
	int 		num_lights = sr.w->lights.size();


	for (int j = 0; j < num_lights; j++) {
		vec3 wi = sr.w->lights[j]->get_direction(sr);
		wi.make_unit_vector();
		sr.normal.make_unit_vector();
		float ndotwi = dot(sr.normal, wi);

		if (ndotwi > 0.0) {
			bool in_shadow = false;

			if ((sr.w)->lights[j]->cast_shadows()) {
				ray shadowray(sr.p, wi);
				in_shadow = sr.w->lights[j]->in_shadows(shadowray, sr);
			}

			if (!in_shadow)
				L += diffuse_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * ndotwi \
				+ specular_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * ndotwi;
			//		lanlog::log_info(to_string(ra.x())+" "+ to_string(ra.y())+" "+ to_string(ra.z()));

		}
	}

	return (L);
}