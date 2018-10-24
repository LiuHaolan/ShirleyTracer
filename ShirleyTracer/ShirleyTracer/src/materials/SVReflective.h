#pragma once

#include "SVPhong.h"
#include "..\BRDF\SVPerfectSpecular.h"

class SVReflective : public SVPhong {
public:

	SVReflective() :SVPhong() {
		reflective_brdf = shared_ptr<SVPerfectSpecularBRDF>(new SVPerfectSpecularBRDF);
		reflective_brdf->set_kr(1.0);
	}
	virtual vec3 shade(hit_record& sr);

	void
		set_kr(float t);

	void
		set_cr(shared_ptr<Texture> t_ptr);

protected:
	shared_ptr<SVPerfectSpecularBRDF> reflective_brdf;

};

inline void SVReflective::set_kr(float t) {
	reflective_brdf->set_kr(t);
}


inline void SVReflective::set_cr(shared_ptr<Texture> t_ptr) {
	reflective_brdf->set_cr(t_ptr);

}