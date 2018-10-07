#pragma once
#include "shaderec.h"

class Light {
public:
	virtual vec3 get_direction(const ShadeRec& sr) const = 0;
	virtual vec3 L(ShadeRec& sr) const = 0;
	void set_shadows(bool shadow) { shadows = shadow; }
	bool cast_shadows() { return shadows; }
	virtual bool in_shadows(const ray& r, const hit_record& sr) const = 0;
	
	virtual float G(const hit_record& sr) const {
		return 1.0;
	}
	virtual float pdf(const hit_record& sr) const {
		return 1.0;
	}


protected:
	bool shadows;
};