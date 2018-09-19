#pragma once
#include "./../integrator.h"

#include "./../utility.h"
#include "./../material.h"

class PTIntegrator : Integrator {
public:
	vec3 Li(const ray& r, hitable *world, int depth);

};

vec3 PTIntegrator::Li(const ray& r, hitable *world, int depth) {
	hit_record rec;

	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
		float pdf;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf)) {

			// hard-coded explicit light sampling
			vec3 on_light = vec3(213 + randd()*(343 - 213), 554, 227 + randd()*(332 - 227));
			vec3 to_light = on_light - rec.p;
			float distance_squared = to_light.squared_length();
			to_light.make_unit_vector();
			if (dot(to_light, rec.normal) < 0)
				return emitted;
			float light_area = (343 - 213)*(332 - 227);
			float light_cosine = fabs(to_light.y());
			if (light_cosine < 0.000001)
				return emitted;
			pdf = distance_squared / (light_area*light_cosine);
			scattered = ray(rec.p, to_light);

			return emitted + attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) / pdf * Li(scattered, world, depth + 1);
		}
		else {
			return emitted;
		}

		//vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		//return 0.5*color(ray(rec.p,target-rec.p),world,depth+1);
	}
	else {
		/*vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);*/
		return vec3(0, 0, 0);
	}
}
