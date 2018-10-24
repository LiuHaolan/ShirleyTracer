#pragma once
#include "..\brdf.h"
#include "..\texture\constant_color.h"

class SVPerfectSpecularBRDF : public BRDF {
public:
	SVPerfectSpecularBRDF() {
		kr = 1.0;
		cr = shared_ptr<Texture>(new ConstantColor(white));
	}

	vec3 sample_f(const hit_record& sr, const vec3& wo, vec3& wi) const;

	void set_kr(const float k) { kr = k; }

//	void set_cr(const shared_ptr<Texture>& c) { cr = c; }

	void set_cr(const float r, const float g, const float b) { cr = shared_ptr<Texture>(new ConstantColor(vec3(r, g, b))); }

	void set_cr(const shared_ptr<Texture> c) { cr = c; }

private:
	float		kr;			// reflection coefficient
	shared_ptr<Texture> 	cr;			// the reflection color
};
