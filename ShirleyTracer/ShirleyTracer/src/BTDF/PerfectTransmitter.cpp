#include "PerfectTransmitter.h"

bool PerfectTransmitter::tir(const hit_record& sr) const {
	vec3 wo = -sr.r.B;
	float cos_thetai = dot(sr.normal, wo)/sr.normal.length()/wo.length();
	float eta = ior;

	if (cos_thetai < 0.0)
		eta = 1.0 / eta;

	return (1.0 - (1.0 - cos_thetai*cos_thetai) / (eta*eta) < 0.0);
}

vec3 PerfectTransmitter::sample_f(const hit_record& sr, const vec3& wo, vec3& wt) const {
	float cos_thetai = dot(sr.topo_normal, wo) / sr.topo_normal.length() / wo.length();
	float eta = ior;
	vec3 n = unit_vector(sr.topo_normal);

	// judge if the ray come from "outside" or "inside"
	if (cos_thetai < 0.0) {
		n = -n;
		cos_thetai = -cos_thetai;
		eta = 1.0 / eta;
	}

	float tmp = 1.0 - (1.0 - cos_thetai*cos_thetai) / (eta*eta);
	float cos_theta2 = sqrt(tmp);

	wt = -unit_vector(wo) / eta - (cos_theta2 - cos_thetai / eta)*n;

	return (kt / (eta*eta)*white / fabs(dot(n, wt)))*n.length()*wt.length();
}