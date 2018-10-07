#pragma once
#include "..\materials.h"
#include "..\texture.h"
#include "..\BRDF\SVLambertian.h"

class SV_Matte : public Material {
public:
	SV_Matte();
	void set_ka(float ka_) { ambient_brdf->set_kd(ka_); }
	void set_kd(float kd_) { diffuse_brdf->set_kd(kd_); }
	void set_cd(Texture* t_ptr);

	vec3 shade(ShadeRec& s);

private:

	SVLambertianBRDF*	ambient_brdf;
	SVLambertianBRDF*	diffuse_brdf;
};


inline void
SV_Matte::set_cd(Texture* t_ptr) {
	ambient_brdf->set_cd(t_ptr);
	diffuse_brdf->set_cd(t_ptr);
}