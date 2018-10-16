#pragma once
#include "Phong.h"
#include "..\BRDF\PerfectSpecular.h"
#include "..\BTDF\PerfectTransmitter.h"

class Transparent : public Phong {
public :
	Transparent() {
		reflective_brdf = new PerfectSpecularBRDF;
		specular_btdf = new PerfectTransmitter;
	}

	vec3 shade(hit_record& r);
	void set_kr(float kr_) { reflective_brdf->set_kr(kr_); }
	void set_kt(float kt_) { specular_btdf->set_kt(kt_); }
	void set_ior(float ior_) {
		specular_btdf->set_ior(ior_);
	}
private:
	PerfectSpecularBRDF* reflective_brdf;
	PerfectTransmitter* specular_btdf;
};