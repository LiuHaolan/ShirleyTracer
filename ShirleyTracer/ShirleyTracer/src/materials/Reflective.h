#pragma once
#include "Phong.h"
#include "..\BRDF\PerfectSpecular.h"
class Reflective : public Phong {
public:
	Reflective():reflective_brdf(new PerfectSpecularBRDF){}

	vec3 shade(hit_record& sr);
	void set_kr(float a) {
		reflective_brdf->set_kr(a);
	}
	void set_cr(vec3 c) {
		reflective_brdf->set_cr(c);
	}
private:
	PerfectSpecularBRDF* reflective_brdf;
};