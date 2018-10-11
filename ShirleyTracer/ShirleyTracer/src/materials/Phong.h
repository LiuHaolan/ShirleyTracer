#pragma once
#include "..\materials.h"
#include "..\BRDF\GlossySpecularBRDF.h"
#include "..\BRDF\LambertianBRDF.h"

class Phong : public Material {
public:
	Phong();

	virtual vec3 shade(hit_record& sr);

	void
		set_ka(const float k);

	void
		set_kd(const float k);

	void
		set_cd(const vec3 c);

	void
		set_cd(const float r, const float g, const float b);

	void
		set_cd(const float c);

	void set_ks(float c);
	void set_exp(float c);

private:
	Lambertian_BRDF*		ambient_brdf;
	Lambertian_BRDF*		diffuse_brdf;
	GlossySpecular_BRDF*	specular_brdf;
};


inline void
Phong::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void
Phong::set_kd(const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void
Phong::set_cd(const vec3 c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
	specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cd

inline void
Phong::set_cd(const float r, const float g, const float b) {
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
	specular_brdf->set_cs(r,g,b);
}

// ---------------------------------------------------------------- set_cd

inline void
Phong::set_cd(const float c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
	specular_brdf->set_cs(c);
}

inline 
void Phong::set_ks(float c) {
	specular_brdf->set_ks(c);
}

inline
void Phong::set_exp(float c) {
	specular_brdf->set_exp(c);
}