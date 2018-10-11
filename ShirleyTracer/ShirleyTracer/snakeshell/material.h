#pragma once
#include "ray.h"
#include "hitable.h"
#include "utility.h"
#include "texture.h"
#include "onb.h"

//struct scatter_record
//{
//	ray specular_ray;
//	bool is_specular;
//	vec3 attenuation;
//	pdf *pdf_ptr;
//};

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, float& pdf) const = 0;
	virtual float scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const {
		std::cout << "wrong call in the virtual function of material" << std::endl;
		return false;
	}
	virtual vec3 emitted(const ray& r_in, const hit_record& rec, float u, float v, const vec3& p) const {
		
		return vec3(0, 0, 0);
	}
};

class lambertian_material : public material {
public:
	lambertian_material(texture* a) :albedo(a) {}
	float scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const{
		float cosine = dot(rec.normal, unit_vector(scattered.direction()));
		if (cosine < 0) cosine = 0;
		return cosine / M_PI;
	}
	
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, float& pdf) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, unit_vector(target - rec.p));

		attenuation = albedo->value(rec.u,rec.v,rec.p);
		pdf = dot(rec.normal, scattered.direction())/M_PI;
		return true;
	}

private:
	texture* albedo;
};

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
}

class metal_material : public material {
public:
	metal_material(const vec3& a, float f):albedo(a),fuzz(f){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), unit_vector(rec.normal));
		scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	vec3 albedo;
	float fuzz;
};

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}


class dielectric : public material {
public:
	dielectric(float ri): ref_idx(ri){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;

		float reflect_prob;
		float cosine;

		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
			if (randd() >= reflect_prob) {
				scattered = ray(rec.p, refracted);
			}
			else
			scattered = ray(rec.p, reflected);
			return true;
		}
		
		scattered = ray(rec.p, reflected);
//		reflect_prob = 1.0;
		

		return true;
	}

	
private:
	float ref_idx;
};

class diffuse_light : public material {
public:
	diffuse_light(texture *a):emit(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered, float& pdf) const { return false; }
	virtual vec3 emitted(const ray& r_in, const hit_record& rec, float u, float v, const vec3& p) const {
		
		if (dot(rec.normal, r_in.direction()) < 0.0)
			return emit->value(u, v, p);
		else
			return vec3(0, 0, 0);
	}
private:
	texture * emit;
};