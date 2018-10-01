#pragma once

#include "ShadeRec.h"
#include "utility.h"

class BRDF {
public:

	//BRDF(void);

	//BRDF(const BRDF& brdf);

	//virtual BRDF*
	//	clone(void)const = 0;

	//BRDF&
	//	operator= (const BRDF& rhs);

	//virtual
	//	~BRDF(void);

	//void
	//	set_sampler(Sampler* sPtr);

	virtual vec3
		f(const ShadeRec& sr, const ShadeRec& wo, const vec3& wi) const { return vec3(0, 0, 0); }

	virtual vec3
		sample_f(const ShadeRec& sr, const vec3& wo, vec3& wi) const { return vec3(0, 0, 0); }

	virtual vec3
		sample_f(const ShadeRec& sr, const vec3& wo, vec3& wi, float& pdf) const { return vec3(0, 0, 0); }

	virtual vec3
		rho(const ShadeRec& sr, const vec3& wo) const { return vec3(0, 0, 0); }


protected:

//	Sampler* sampler_ptr;		// for indirect illumination
};


