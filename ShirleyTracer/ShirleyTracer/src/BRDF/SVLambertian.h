#pragma once

#include "..\brdf.h"
#include "..\texture\constant_color.h"

class SVLambertianBRDF : public BRDF {
public:
	SVLambertianBRDF() :kd(new ConstantColor(vec3(0.0, 0.0, 0.0))), cd(new ConstantColor(vec3(0.0,0.0,0.0))) {}
	SVLambertianBRDF(Texture* kd_, Texture* cd_) :kd(kd_), cd(cd_) {}

	vec3 f(const hit_record& sr, const vec3& wo, const vec3& wi) const;

	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const;

	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi, float& pdf) const;

	vec3 rho(const hit_record& sr, const vec3& wo) const;

	void set_cd(shared_ptr<Texture> ptr) { cd = ptr; }
	void set_kd(shared_ptr<Texture> kd_) { kd = kd_; }

private:

	shared_ptr<Texture>		kd;
	shared_ptr<Texture> cd;
};
