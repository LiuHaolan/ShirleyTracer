#pragma once

#include "..\materials\Phong.h"

class GlossyReflective : public Phong {
public:
	GlossyReflective() { glossy_specular_brdf = new GlossySpecular_BRDF; }

	vec3 shade(hit_record& sr) { return black; }
	vec3 area_light_shade(hit_record& sr);
	void set_samples(const int num_samples, const float exp) {
	//	Jittered* sampler = new Jittered(num_samples);
		glossy_specular_brdf->set_samples(num_samples,exp);
	}
	void set_kr(const float k) {
		glossy_specular_brdf->set_ks(k);
	}
	void set_exponent(const float exp) {
		glossy_specular_brdf->set_exp(exp);
	}
	void set_cr(const vec3& c) {
		glossy_specular_brdf->set_cs(c);
	}

private:
	GlossySpecular_BRDF* glossy_specular_brdf;

};
