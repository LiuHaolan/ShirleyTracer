#pragma once
#include "..\light.h"
#include "..\materials\Emissive.h"

class AreaLight : public Light {
public:
	vec3 get_direction(const hit_record& sr) const;
	vec3 L(hit_record& sr) const;	bool in_shadows(const ray& r, const hit_record& sr) const;	float G(const hit_record& sr) const {		float ndotd = dot(light_normal, wi);		float d2 = (sample_point - sr.p).squared_length();		return (ndotd / d2);	}	float pdf(const hit_record& sr) const {
		return (object_ptr->pdf(sr));	}
	void set_object(hitable* ptr) { object_ptr = ptr; }
	void set_Emissive(Emissive* ptr) { material_ptr = ptr; }
private:
	hitable* object_ptr;
	Emissive* material_ptr;
	mutable vec3 sample_point;
	mutable vec3 light_normal;
	mutable vec3 wi;
};