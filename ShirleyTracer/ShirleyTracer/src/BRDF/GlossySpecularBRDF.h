#pragma once
#include "..\brdf.h"
#include "../sampler/Jittered.h"

class GlossySpecular_BRDF : public BRDF {
public:
	GlossySpecular_BRDF():ks(0), cs(0), exp(0){}

	vec3 f(const hit_record& sr, const vec3& wo, const vec3& wi) const;
	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf);

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

	void set_samples(const int nums, const float exp) {
		sampler_ptr = new Jittered(nums);
		sampler_ptr->map_samples_to_hemisphere(exp);
	}

private:
	float		ks;
	vec3 	cs;			// specular color
	float		exp; 		// specular exponent
};
