#pragma once
#include "..\brdf.h"

class PerfectSpecularBRDF : public BRDF {
public:
	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const;

	void set_kr(const float k) { kr = k; }

	void set_cr(const vec3& c) { cr = c; }

	void set_cr(const float r, const float g, const float b) { cr = vec3(r, g, b); }

	void set_cr(const float c) { cr = c; }

private:
	float		kr;			// reflection coefficient
	vec3 	cr;			// the reflection colour
};