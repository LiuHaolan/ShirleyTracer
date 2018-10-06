#pragma once
#include "shaderec.h"

class Light {
public:
	virtual vec3 get_direction(ShadeRec& sr) const = 0;
	virtual vec3 L(ShadeRec& sr) const = 0;
	void set_shadows(bool shadow) { shadows = shadow; }
	bool cast_shadows() { return shadows; }
	bool in_shadows(const ray& r, const hit_record& sr) const;

protected:
	bool shadows;
};