// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "FresnelReflector.h"

// ---------------------------------------------------------- default constructor

FresnelReflector::FresnelReflector(void)
	: 	BRDF(),
		kr(0.0),
		cr(1.0)
{}

// ---------------------------------------------------------- destructor

FresnelReflector::~FresnelReflector(void) {}


// ---------------------------------------------------------------------- clone

FresnelReflector*
FresnelReflector::clone(void) const {
	return (new FresnelReflector(*this));
}


// ----------------------------------------------------------------------------------------- fresnel

float
FresnelReflector::fresnel(const hit_record& sr) const {
	vec3 normal(sr.normal);
	float ndotd = -dot(normal , sr.r.B)/normal.length()/sr.r.B.length();
	float eta;

	if (ndotd < 0.0) {
		normal = -normal;
		eta = eta_out / eta_in;
	}
	else
		eta = eta_in / eta_out;

	float cos_theta_i 		= -dot(normal, sr.r.B)/normal.length()/sr.r.B.length();
	float temp 				= 1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta);
	float cos_theta_t 		= sqrt (1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta));
	float r_parallel 		= (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
	float r_perpendicular 	= (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
	float kr 				= 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

	return (kr);
}


// ---------------------------------------------------------- f

vec3
FresnelReflector::f(const hit_record& sr, const vec3& wo, const vec3& wi) const {
	return (black);
}


// ----------------------------------------------------------------------------------------- sample_f

vec3 FresnelReflector::sample_f(const hit_record& sr, const vec3& wo, vec3& wr) const {
	float ndotwo = dot(sr.normal, wo)/sr.normal.length()/wo.length();
	wr = -wo + 2.0 * sr.normal * ndotwo;
	return (fresnel(sr) * white / fabs(dot(sr.normal , wr)/sr.normal.length()/wr.length()));
}

// ---------------------------------------------------------- rho

vec3
FresnelReflector::rho(const hit_record& sr, const vec3& wo) const {
	return (black);
}
