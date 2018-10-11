#include "SV_Matte.h"

vec3 SV_Matte::shade(hit_record& sr) {
	vec3 	wo = -sr.r.B;
	vec3 	L = ambient_brdf->rho(sr, wo) * sr.w->ambient_ptr->L(sr);
	int 		num_lights = sr.w->lights.size();

	for (int j = 0; j < num_lights; j++) {
		Light* light_ptr = sr.w->lights[j];
		vec3 wi = light_ptr->get_direction(sr);
		wi.make_unit_vector();
		float ndotwi = dot(sr.normal, wi);
		float ndotwo = dot(sr.normal, wo);

		if (ndotwi > 0.0 && ndotwo > 0.0) {
			bool in_shadow = false;

			if (sr.w->lights[j]->cast_shadows()) {
				ray shadow_ray(sr.p, wi);
				in_shadow = light_ptr->in_shadows(shadow_ray, sr);
			}

			if (!in_shadow)
				L += diffuse_brdf->f(sr, wo, wi) * light_ptr->L(sr) * light_ptr->G(sr) * ndotwi;
		}
	}

	return (L);
}