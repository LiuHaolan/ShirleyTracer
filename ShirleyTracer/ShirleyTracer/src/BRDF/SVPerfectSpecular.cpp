#include "SVPerfectSpecular.h"

vec3 SVPerfectSpecularBRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const {
	vec3 normal = unit_vector(sr.normal);
	vec3 woo = unit_vector(wo);
	float ndotwo = dot(normal, woo);
	wi = -woo + 2 * normal * ndotwo;

	return (kr*((cr)->value(sr)) / dot(normal, wi));
}