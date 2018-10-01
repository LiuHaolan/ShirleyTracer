#include "LambertianBRDF.h"

vec3
Lambertian_BRDF::f(const ShadeRec& sr, const vec3& wo, const vec3& wi) const { return (kd * cd * INV_PI); }

// need to implement using path tracing
vec3
Lambertian_BRDF::sample_f(const ShadeRec& sr, const vec3& wo, vec3& wi) const { return vec3(0, 0, 0); }

vec3
Lambertian_BRDF::sample_f(const ShadeRec& sr, const vec3& wo, vec3& wi, float& pdf) const { return vec3(0, 0, 0); }

vec3
Lambertian_BRDF::rho(const ShadeRec& sr, const vec3& wo) const { return kd*cd; }