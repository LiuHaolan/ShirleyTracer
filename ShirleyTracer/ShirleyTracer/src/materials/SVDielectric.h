#pragma once

#include "SVPhong.h"
#include "..\BRDF\FresnelReflector.h"
#include "..\BTDF\FresnelTransmitter.h"

class SVDielectric : public SVPhong
{
public:

	SVDielectric(void) :SVPhong() {
		fresnel_brdf = shared_ptr<FresnelReflector>(new FresnelReflector);
		fresnel_btdf = shared_ptr<FresnelTransmitter>(new FresnelTransmitter);
	}

	void
		set_eta_in(const float ei);

	void
		set_eta_out(const float eo);

	void
		set_cf_in(const vec3& ci);

	void
		set_cf_in(const float ri, const float gi, const float bi);

	void
		set_cf_in(const float ci);

	void
		set_cf_out(const vec3& co);

	void
		set_cf_out(const float ro, const float go, const float bo);

	void
		set_cf_out(const float co);

	virtual vec3
		shade(hit_record& s);

private:

	vec3 			cf_in;			// interior filter color
	vec3 			cf_out;			// exterior filter color

	shared_ptr<FresnelReflector> fresnel_brdf;
	shared_ptr<FresnelTransmitter> fresnel_btdf;
};



// -------------------------------------------------------------- set_eta_in

inline void
SVDielectric::set_eta_in(const float ei) {
	fresnel_brdf->set_eta_in(ei);
	fresnel_btdf->set_eta_in(ei);
}


// -------------------------------------------------------------- set_eta_out

inline void
SVDielectric::set_eta_out(const float eo) {
	fresnel_brdf->set_eta_out(eo);
	fresnel_btdf->set_eta_out(eo);
}


// -------------------------------------------------------------- set_cf_in

inline void
SVDielectric::set_cf_in(const vec3& ci) {
	cf_in = ci;
}


// -------------------------------------------------------------- set_cf_in

inline void
SVDielectric::set_cf_in(const float ri, const float gi, const float bi) {
	cf_in[0] = ri; cf_in[1] = gi; cf_in[2] = bi;
}


// -------------------------------------------------------------- set_cf_in

inline void
SVDielectric::set_cf_in(const float ci) {
	set_cf_in(ci, ci, ci);
}


// -------------------------------------------------------------- set_cf_out

inline void
SVDielectric::set_cf_out(const vec3& co) {
	cf_out = co;
}


// -------------------------------------------------------------- set_cf_out

inline void
SVDielectric::set_cf_out(const float ro, const float go, const float bo) {
	cf_out[0] = ro; cf_out[1] = go; cf_out = bo;
}


// -------------------------------------------------------------- set_cf_out

inline void
SVDielectric::set_cf_out(const float co) {
	set_cf_out(co,co,co);
}
