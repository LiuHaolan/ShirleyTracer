#include "SVGlossySpecular.h"
#include "..\onb.h"

vec3 SVGlossySpecular::sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const{
	float ndotwo = dot(sr.normal, wo) / wo.length() / sr.normal.length();

	vec3 r = -unit_vector(wo) + 2.0*unit_vector(sr.normal)*ndotwo;

	onb cor;
	cor.build_from_w(r);

	vec3 sp = sampler_ptr->sample_hemisphere();
	wi = cor.local(sp);

	if (dot(sr.normal, wi) < 0) {
		wi = cor.local(-sp.x(), -sp.y(), sp.z());
	}

	float phong_lobe = pow(dot(r, wi) / r.length() / wi.length(), exp);
	pdf = phong_lobe * dot(unit_vector(sr.normal), unit_vector(wi));

	vec3 kcol = ks->value(sr);
	vec3 ccol = cs->value(sr);
	return(kcol*ccol *phong_lobe);
}

vec3 SVGlossySpecular::f(const hit_record& sr, const vec3& wo, const vec3& wi) const {
	vec3 	L;
	vec3 norm = unit_vector(sr.normal);
	vec3 unitwi = unit_vector(wi);
	float 		ndotwi = dot(norm, unitwi);
	vec3 	r = -unitwi + 2.0 * norm * ndotwi; // mirror reflection direction
	r.make_unit_vector();
	vec3 unitwo = unit_vector(wo);
	float 		rdotwo = dot(r, unitwo);

	vec3 kcol = ks->value(sr);
	vec3 ccol = cs->value(sr);

	if (rdotwo > 0.0) {
		L = kcol * ccol * pow(rdotwo, exp);
		float k = 1.0;
	}
	return (L);
}