#include "matte_material.h"
#include "..\BRDF\LambertianBRDF.h"

Matte::Matte() {
	ambient_brdf = new Lambertian_BRDF;
	diffuse_brdf = new Lambertian_BRDF;
}

vec3 Matte::shade(ShadeRec& sr) {
	return vec3(0, 0, 0);
	vec3 wo = -sr.r.B;
	vec3 	L = ambient_brdf->rho(sr, wo) * sr.w->ambient_ptr->L(sr);
	int 		num_lights = sr.w->lights.size();

	for (int j = 0; j < num_lights; j++) {
		vec3 wi = sr.w->lights[j]->get_direction(sr);
		float ndotwi = dot(sr.normal, wi);

		if (ndotwi > 0.0)
			L += diffuse_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * ndotwi;
	}

	return (L);
}