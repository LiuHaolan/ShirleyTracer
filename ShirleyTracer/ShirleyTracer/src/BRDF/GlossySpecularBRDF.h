#pragma once
#include "..\brdf.h"

class GlossySpecular_BRDF : public BRDF {
public:
	vec3 f(const hit_record& sr, const vec3& wo, const vec3& wi) const;

	void
		set_ks(const float ks);

	void
		set_exp(const float exp);

	void
		set_cs(const vec3& c);

	void
		set_cs(const float r, const float g, const float b);

	void
		set_cs(const float c);

private:
	float		ks;
	vec3 	cs;			// specular color
	float		exp; 		// specular exponent
};
