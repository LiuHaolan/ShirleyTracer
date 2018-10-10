#pragma once


#include "..\hitable.h"

class Disk : public hitable {
public:	
	Disk(vec3 a_,vec3 n_,float r_):a(a_),normal(n_),radius(r_){}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

private:
	vec3 a;
	vec3 normal;
	float radius;
};