#include "SVLambertian.h"



vec3 SVLambertianBRDF::f(const hit_record& sr, const vec3& wo, const vec3& wi) const {
	return (kd->value(sr)*cd->value(sr)*INV_PI);
}

vec3 SVLambertianBRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const {
	return vec3(0, 0, 0);
}

vec3 SVLambertianBRDF::sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const {
	return vec3(0, 0, 0);
}

vec3 SVLambertianBRDF::rho(const hit_record& sr, const vec3& wo) const {
	return (kd->value(sr)*(cd->value(sr)));
}