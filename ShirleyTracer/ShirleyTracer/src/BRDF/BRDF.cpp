#include "LambertianBRDF.h"
#include "GlossySpecularBRDF.h"
#include "PerfectSpecular.h"

vec3
Lambertian_BRDF::f(const hit_record& sr, const vec3& wo, const vec3& wi) const { return (kd * cd * INV_PI); }

// need to implement using path tracing
vec3
Lambertian_BRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const { return vec3(0, 0, 0); }

vec3
Lambertian_BRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const { return vec3(0, 0, 0); }

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

