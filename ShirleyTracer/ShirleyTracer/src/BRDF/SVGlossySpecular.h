#pragma once

#include "..\brdf.h"
#include "..\texture.h"

class SVGlossySpecular : public BRDF {
public:
	vec3 f(const hit_record& sr, const vec3& wo, const vec3& wi) const;

	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const { return vec3(0, 0, 0); }

	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const;

	vec3 rho(const hit_record& sr, const vec3& wo) const { return vec3(0, 0, 0); }
	
	void
		set_ks(shared_ptr<Texture> t_ptr);

	void
		set_exp(const float exp);

	void
		set_cs(shared_ptr<Texture> t_ptr);

private:


	shared_ptr<Texture>	ks;
	shared_ptr<Texture> 	cs;			    // specular color
	float		exp; 		    // specular exponent
};


inline void
SVGlossySpecular::set_ks(shared_ptr<Texture> t_ptr) {
	ks = t_ptr;
}

// -------------------------------------------------------------- set_exp

inline void
SVGlossySpecular::set_exp(const float e) {
	exp = e;
}


// -------------------------------------------------------------- set_cs

inline void
SVGlossySpecular::set_cs(shared_ptr<Texture> t_ptr) {
	cs = t_ptr;
}