#pragma once
#include "../materials.h"

#include "../BRDF/LambertianBRDF.h"

class Matte : public Material {
public:
	Matte();

	vec3 shade(hit_record& src);
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
	vec3 area_light_shade(hit_record& sr);
	vec3 path_shade(hit_record& sr);
	vec3 global_path_shade(hit_record& sr);

	void set_sampler(Sampler* s) {
		diffuse_brdf->set_cosine_sampler(s);
	}

private:
	Lambertian_BRDF* ambient_brdf;
	Lambertian_BRDF* diffuse_brdf;

};



inline void
Matte::set_ka(const float ka) {
	ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void
Matte::set_kd(const float kd) {
	diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

inline void
Matte::set_cd(const vec3 c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd

inline void
Matte::set_cd(const float r, const float g, const float b) {
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
}

// ---------------------------------------------------------------- set_cd

inline void
Matte::set_cd(const float c) {
	ambient_brdf->set_cd(c);
	diffuse_brdf->set_cd(c);
}


