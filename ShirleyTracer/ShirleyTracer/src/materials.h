#pragma once

#include "World.h"
// notice: this form is the real Material class

class Material {
public:
	virtual vec3 shade(ShadeRec& sr)= 0;
	virtual vec3 area_light_shade(ShadeRec& sr) { return vec3(0, 0, 0); }
	virtual vec3 path_light_shade(ShadeRec& sr) { return vec3(0, 0, 0); }
//	virtual vec3 area_light_shade(hit_record& sr);
};