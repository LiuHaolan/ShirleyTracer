#pragma once
#include "..\light.h"
#include "..\materials\Emissive.h"

class AreaLight : public Light {
public:
	virtual vec3 get_direction(const hit_record& sr) const;
	virtual vec3 L(ShadeRec& sr) const;	virtual bool in_shadows(const ray& r, const ShadeRec& sr) const;	virtual float G(const hit_record& sr) const;	virtual float pdf(const hit_record& sr) const;
	void set_object(hitable* ptr) { object_ptr = ptr; }

private:
	hitable* object_ptr;
	Emissive* material_ptr;
	mutable vec3 sample_point;
	vec3 light_normal;
	vec3 wi;
};