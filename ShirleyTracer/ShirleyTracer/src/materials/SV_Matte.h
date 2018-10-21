#pragma once
#include "..\materials.h"
#include "..\texture.h"
#include "..\BRDF\SVLambertian.h"

class SV_Matte : public Material {
public:
	SV_Matte() {
		ambient_brdf = new SVLambertianBRDF;
		diffuse_brdf = new SVLambertianBRDF;
	}
	void set_ka(float ka_) { ambient_brdf->set_kd(ka_); }
	void set_kd(float kd_) { diffuse_brdf->set_kd(kd_); }
	void set_cd(shared_ptr<Texture> t_ptr);

	vec3 shade(hit_record& s);
	vec3 area_light_shade(hit_record& sr);
private:

	SVLambertianBRDF*	ambient_brdf;
	SVLambertianBRDF*	diffuse_brdf;
};


inline void
SV_Matte::set_cd(shared_ptr<Texture> t_ptr) {
	ambient_brdf->set_cd(t_ptr);
	diffuse_brdf->set_cd(t_ptr);
}