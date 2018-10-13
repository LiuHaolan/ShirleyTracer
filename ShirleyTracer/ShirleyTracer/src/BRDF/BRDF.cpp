#include "LambertianBRDF.h"
#include "GlossySpecularBRDF.h"
#include "PerfectSpecular.h"

#include "..\onb.h"


vec3
Lambertian_BRDF::f(const hit_record& sr, const vec3& wo, const vec3& wi) const { return (kd * cd * INV_PI); }

// need to implement using path tracing
vec3
Lambertian_BRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const { 
	lanlog::log_error("Lambertian BRDF unimplemented");
	return vec3(0, 0, 0); 
}

vec3
Lambertian_BRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const { 
	
	vec3 w = unit_vector(sr.normal);
	onb cor;
	cor.build_from_w(w);

	vec3 sp = sampler_ptr->sample_hemisphere();
	wi = cor.local(sp[0], sp[1], sp[2]);
	wi.make_unit_vector();
	pdf = dot(w, wi) * INV_PI;

	return kd*cd*INV_PI; 
}

vec3
Lambertian_BRDF::rho(const hit_record& sr, const vec3& wo) const { return kd*cd; }


vec3
GlossySpecular_BRDF::f(const hit_record& sr, const vec3& wo, const vec3& wi) const {
	vec3 	L;
	vec3 norm = unit_vector(sr.normal);
	vec3 unitwi = unit_vector(wi);
	float 		ndotwi = dot(norm , unitwi);
	vec3 	r = -unitwi + 2.0 * norm * ndotwi; // mirror reflection direction
	r.make_unit_vector();
	vec3 unitwo = unit_vector(wo);
	float 		rdotwo = dot(r, unitwo);


	if (rdotwo > 0.0) {
		L = ks * cs * pow(rdotwo, exp);
		float k = 1.0;
	}
	return (L);
}

void
GlossySpecular_BRDF::set_ks(const float k){
	ks = k;
}

void
GlossySpecular_BRDF::set_exp(const float ex) {
	exp = ex;
}

void
GlossySpecular_BRDF::set_cs(const vec3& c) {
	cs = c;
}

void
GlossySpecular_BRDF::set_cs(const float r, const float g, const float b) {
	cs = vec3(r, g, b);
}

void
GlossySpecular_BRDF::set_cs(const float c) {
	cs = vec3(c, c, c);
}

vec3 PerfectSpecularBRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const {
	vec3 normal = unit_vector(sr.normal);
	vec3 woo = unit_vector(wo);
	float ndotwo = dot(normal , woo);
	wi = -woo + 2 * normal * ndotwo;

	return (kr*cr / dot(normal,wi));
}

vec3 GlossySpecular_BRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) {
	float ndotwo = dot(sr.normal, wo) / wo.length() / sr.normal.length();

	vec3 r = -unit_vector(wo) + 2.0*unit_vector(sr.normal)*ndotwo;

	onb cor;
	cor.build_from_w(r);

	vec3 sp = sampler_ptr->sample_hemisphere();
	wi = cor.local(sp);

	if (dot(sr.normal, wi) < 0) {
		wi = cor.local(-sp.x(), - sp.y(), sp.z());
	}

	float phong_lobe = pow(dot(r,wi)/r.length()/wi.length(), exp);
	pdf = phong_lobe * dot(unit_vector(sr.normal), unit_vector(wi));

	return(ks*cs *phong_lobe);
}

