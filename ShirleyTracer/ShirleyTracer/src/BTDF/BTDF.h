#pragma once
#include "..\brdf.h"
class BTDF{
public:

	virtual bool tir(const hit_record& sr) const = 0;
	virtual vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const = 0;

};