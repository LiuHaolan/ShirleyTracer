#include "AreaLight.h"


vec3 AreaLight::get_direction(const hit_record& sr) const{
	sample_point = object_ptr->sample();
	vec3 light_normal = object_ptr->get_normal(sample_point);
	vec3 wi = sample_point - sr.p;
	wi.make_unit_vector();

	return wi;
 }
vec3 AreaLight::L(ShadeRec& sr) const {	float ndotd = -dot(light_normal,wi);	if (ndotd > 0.0)		return material_ptr->get_Le(sr);	else		return black;}bool AreaLight::in_shadows(const ray& r, const ShadeRec& sr) const {	float t;	int num_objects = sr.w->objects.size();	float ts = dot((sample_point - r.A) , r.B);	for(int j=0;j<num_objects;j++)		if (sr.w->objects[j]->hitP(r, t) && t < ts) {			return true;		}	return false;}float AreaLight::G(const hit_record& sr) const {	float ndotd = -dot(light_normal , wi);	float d2 = (sample_point-sr.p).squared_length();	return (ndotd / d2);}float AreaLight::pdf(const hit_record& sr) const {
	return (object_ptr->pdf(sr));}