#pragma once


#include "..\brdf.h"

class Lambertian_BRDF : public BRDF {
public:
	Lambertian_BRDF():kd(0),cd(vec3(0,0,0)){}
	Lambertian_BRDF(float kd_, vec3 cd_):kd(kd_),cd(cd_){}

	vec3
		f(const hit_record& sr, const vec3& wo, const vec3& wi) const;

	vec3
		sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const;

	vec3
		sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const;

	vec3
		rho(const hit_record& sr, const vec3& wo) const;

	void
		set_ka(const float ka);

	void
		set_kd(const float kd);

	void
		set_cd(const vec3& c);

	void
		set_cd(const float r, const float g, const float b);

	void
		set_cd(const float c);


private:

	float		kd;
	vec3 	cd;
};


// -------------------------------------------------------------- set_ka

inline void
Lambertian_BRDF::set_ka(const float k) {
	kd = k;
}



// -------------------------------------------------------------- set_kd

inline void
Lambertian_BRDF::set_kd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- set_cd

inline void
Lambertian_BRDF::set_cd(const vec3& c) {
	cd = c;
}


// ---------------------------------------------------------------- set_cd

inline void
Lambertian_BRDF::set_cd(const float r, const float g, const float b) {
	cd[0] = r; cd[1] = g; cd[2] = b;
}


// ---------------------------------------------------------------- set_cd

inline void
Lambertian_BRDF::set_cd(const float c) {
	cd[0] = c; cd[1] = c; cd[2] = c;
}
