#ifndef FRESNELTRANSMITTER_H
#define FRESNELTRANSMITTER_H

#include "BTDF.h"

class FresnelTransmitter: public BTDF {
	public:

		FresnelTransmitter(void);

		FresnelTransmitter(const FresnelTransmitter& ft);

		virtual FresnelTransmitter*
		clone(void);

		~FresnelTransmitter(void);

		FresnelTransmitter&
		operator= (const FresnelTransmitter& rhs);

		void
		set_eta_in(const float ei);

		void
		set_eta_out(const float eo);

		bool
		tir(const hit_record& sr) const;

		virtual vec3
		f(const hit_record& sr, const vec3& wo, const vec3& wi) const;

		virtual vec3
		sample_f(const hit_record& sr, const vec3& wo, vec3& wt) const;

		virtual vec3
		rho(const hit_record& sr, const vec3& wo) const;

        virtual float
        fresnel(const hit_record& sr) const;
    
        float
        get_eta(const hit_record& sr, const vec3& wo) const;

	private:

		float	eta_in;
		float	eta_out;
};



// -------------------------------------------------------------- set_eta_in

inline void
FresnelTransmitter::set_eta_in(const float ei) {
	eta_in = ei;
}


// -------------------------------------------------------------- set_eta_out

inline void
FresnelTransmitter::set_eta_out(const float eo) {
	eta_out = eo;
}
#endif // FRESNELTRANSMITTER_H
