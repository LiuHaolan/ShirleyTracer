#ifndef FRESNELREFLECTOR_H
#define FRESNELREFLECTOR_H


// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// this implements perfect specular reflection for indirect illumination
// with reflective materials

#include "..\BRDF.h"


class FresnelReflector: public BRDF
{
	public:

		FresnelReflector(void);

		~FresnelReflector(void);

		virtual FresnelReflector*
		clone(void) const;

		void
		set_kr(const float k);

		void
		set_eta_in(const float ei);

		void
		set_eta_out(const float eo);

		void
		set_cr(const vec3& c);

		void
		set_cr(const float r, const float g, const float b);

		void
		set_cr(const float c);

		virtual vec3
		f(const hit_record& sr, const vec3& wo, const vec3& wi) const;

		virtual vec3
		sample_f(const hit_record& sr, const vec3& wo, vec3& wr) const;

		virtual vec3
		rho(const hit_record& sr, const vec3& wo) const;

        virtual float
        fresnel(const hit_record& sr) const;
	private:

		float		kr;			// reflection coefficient
		vec3 	cr;			// the reflection colour
		float		eta_in;
		float		eta_out;
};


// -------------------------------------------------------------- set_kr

inline void
FresnelReflector::set_kr(const float k) {
	kr = k;
}


// -------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const vec3& c) {
	cr = c;
}


// ---------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const float r, const float g, const float b) {
	cr[0] = r; cr[1] = g; cr[2] = b;
}


// ---------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const float c) {
	cr[0] = c; cr[1] = c; cr[2] = c;
}


// -------------------------------------------------------------- set_eta_in

inline void
FresnelReflector::set_eta_in(const float ei) {
	eta_in = ei;
}


// -------------------------------------------------------------- set_eta_out

inline void
FresnelReflector::set_eta_out(const float eo) {
	eta_out = eo;
}
#endif // FRESNELREFLECTOR_H
