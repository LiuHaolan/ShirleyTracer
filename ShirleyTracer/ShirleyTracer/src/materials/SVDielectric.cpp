#include "SVDielectric.h"

vec3
SVDielectric::shade(hit_record& sr) {
	vec3 L(SVPhong::shade(sr));

	vec3 	wi;
	vec3 	wo(-sr.r.B);
	vec3 	fr = fresnel_brdf->sample_f(sr, wo, wi);  	// computes wi
	ray 		reflected_ray(sr.p, wi);

//	double 		t;
	vec3 	Lr, Lt;
	sr.normal.make_unit_vector();
	wi.make_unit_vector();
	float 		ndotwi = dot(sr.normal, wi);

	if (fresnel_btdf->tir(sr)) {								// total internal reflection
		if (ndotwi < 0.0) {
			// reflected ray is inside

			Lr = sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1);
			L += cf_in.powc(sr.t) * Lr;   						// inside filter color
		}
		else {
			// reflected ray is outside

			Lr = sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1);
			L += cf_out.powc(sr.t) * Lr;   					// outside filter color
		}
	}
	else { 													// no total internal reflection
		vec3 wt;
		vec3 ft = fresnel_btdf->sample_f(sr, wo, wt);  	// computes wt
		ray transmitted_ray(sr.p, wt);

		wt.make_unit_vector();
		float ndotwt = dot(sr.normal, wt);

		if (ndotwi < 0.0) {
			// reflected ray is inside

			Lr = fr * sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1) * fabs(ndotwi);
			L += cf_in.powc(sr.t) * Lr;     					// inside filter color

			// transmitted ray is outside

			Lt = ft * sr.w->integrator_ptr->Li(transmitted_ray, sr.depth + 1) * fabs(ndotwt);
			L += cf_out.powc(sr.t) * Lt;   					// outside filter color
		}
		else {
			// reflected ray is outside

			Lr = fr * sr.w->integrator_ptr->Li(reflected_ray, sr.depth + 1) * fabs(ndotwi);
			L += cf_out.powc(sr.t) * Lr;						// outside filter color

			// transmitted ray is inside

			Lt = ft * sr.w->integrator_ptr->Li(transmitted_ray, sr.depth + 1) * fabs(ndotwt);
			L += cf_in.powc(sr.t) * Lt; 						// inside filter color
		}
	}

	return (L);
}
