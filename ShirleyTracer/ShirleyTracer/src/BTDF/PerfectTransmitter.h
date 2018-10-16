#pragma once
#include "BTDF.h"

class PerfectTransmitter : public BTDF {
public:
	PerfectTransmitter() { kt = 1.0; }
	PerfectTransmitter(float kt_,float ior_) : kt(kt_),ior(ior_){}
	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wt) const;

	bool tir(const hit_record& sr) const;
	void set_ior(float ior_) { ior = ior_; }
	void set_kt(float kt_) { kt = kt_; }
private:
	float kt, ior;
};