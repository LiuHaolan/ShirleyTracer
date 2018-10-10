#include "matte_material.h"
#include "..\BRDF\LambertianBRDF.h"
#include "..\BRDF\GlossySpecularBRDF.h"
#include "Phong.h"

#include "..\lanlog.h"


Matte::Matte() {
	ambient_brdf = new Lambertian_BRDF;
	diffuse_brdf = new Lambertian_BRDF;
}

vec3 Matte::shade(hit_record& sr) {

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
			if(!in_shadow)
				L += diffuse_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * ndotwi;
		}
	}

	return (L);
}

vec3 Matte::area_light_shade(hit_record& sr) {
	vec3 	wo = -sr.r.B;
	vec3 	L = ambient_brdf->rho(sr, wo) * sr.w->ambient_ptr->L(sr);
	int 		num_lights = sr.w->lights.size();

	for (int j = 0; j < num_lights; j++) {
		vec3 	wi = sr.w->lights[j]->get_direction(sr);
		float 		ndotwi = dot(sr.normal, wi);

		if (ndotwi > 0.0) {
			bool in_shadow = false;

			if (sr.w->lights[j]->cast_shadows()) {
				ray shadow_ray(sr.p, wi);
				in_shadow = sr.w->lights[j]->in_shadows(shadow_ray, sr);
			}

			if (!in_shadow) {
	//			vec3 radiance = diffuse_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * sr.w->lights[j]->G(sr) * ndotwi / sr.w->lights[j]->pdf(sr);
	//			AreaLight* ptr = static_cast<AreaLight*>(sr.w->lights[j]);

				//vec3 s1 = sr.w->lights[j]->L(sr);
				//float s2 = sr.w->lights[j]->G(sr);
				//float s3 = ndotwi / sr.w->lights[j]->pdf(sr);
				vec3 radiance = diffuse_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * sr.w->lights[j]->G(sr) * ndotwi / sr.w->lights[j]->pdf(sr);
				radiance = vec3(abs(radiance[0]), abs(radiance[1]), abs(radiance[2]));
				L += radiance;
			}
		}
	}

	return (L);
}

vec3 Phong::shade(hit_record& sr) {
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

			if(!in_shadow)
				L += diffuse_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * ndotwi \
			        +specular_brdf->f(sr, wo, wi) * sr.w->lights[j]->L(sr) * ndotwi;
	//		lanlog::log_info(to_string(ra.x())+" "+ to_string(ra.y())+" "+ to_string(ra.z()));

		}
	}

	return (L);
}

Phong::Phong() {
	ambient_brdf = new Lambertian_BRDF;
	diffuse_brdf = new Lambertian_BRDF;
	specular_brdf = new GlossySpecular_BRDF;
}