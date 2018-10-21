#pragma once
#pragma once
#include "..\materials.h"
#include "..\BRDF\SVGlossySpecular.h"
#include "..\BRDF\SVLambertian.h"

class SVPhong : public Material {
public:
	SVPhong() {
		ambient_brdf = shared_ptr<SVLambertianBRDF>(new SVLambertianBRDF);
		diffuse_brdf = shared_ptr<SVLambertianBRDF>(new SVLambertianBRDF);
		specular_brdf = shared_ptr<SVGlossySpecular>(new SVGlossySpecular);
		ambient_brdf->set_kd(1.0);
		diffuse_brdf->set_kd(1.0);
	}

	virtual vec3 shade(hit_record& sr);
	virtual vec3 area_light_shade(hit_record& sr) {
		lanlog::log_error("Error arealight of SVPhong shading is not implemented.");
		return vec3(0, 0, 0);
	}

	void
		set_ka(shared_ptr<Texture> t_ptr);

	void
		set_kd(shared_ptr<Texture> t_ptr);

	void
		set_ks(shared_ptr<Texture> t_ptr);

	void
		set_exp(const float exp);

	void
		set_cd(shared_ptr<Texture> t_ptr);

	void
		set_cs(shared_ptr<Texture> t_ptr);

private:
	shared_ptr<SVLambertianBRDF>		ambient_brdf;
	shared_ptr<SVLambertianBRDF>		diffuse_brdf;
	shared_ptr<SVGlossySpecular>	specular_brdf;
};

inline void
SVPhong::set_ka(shared_ptr<Texture> t_ptr) {
	ambient_brdf->set_cd(t_ptr);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object

inline void
SVPhong::set_kd(shared_ptr<Texture> t_ptr) {
	diffuse_brdf->set_cd(t_ptr);
}


// ---------------------------------------------------------------- set_ks
// this sets GlossySpecular::ks

inline void
SVPhong::set_ks(shared_ptr<Texture> t_ptr) {
	specular_brdf->set_ks(t_ptr);
}

// ---------------------------------------------------------------- set_exp
// this sets GlossySpecular::ks

inline void
SVPhong::set_exp(const float exp) {
	specular_brdf->set_exp(exp);
}


// ---------------------------------------------------------------- set_cd

inline void
SVPhong::set_cd(shared_ptr<Texture> t_ptr) {
	ambient_brdf->set_cd(t_ptr);//one pointer for one memory
	diffuse_brdf->set_cd(t_ptr);
	specular_brdf->set_cs(t_ptr);
}


// ---------------------------------------------------------------- set_cs

inline void
SVPhong::set_cs(shared_ptr<Texture> t_ptr) {
	specular_brdf->set_cs(t_ptr);
}