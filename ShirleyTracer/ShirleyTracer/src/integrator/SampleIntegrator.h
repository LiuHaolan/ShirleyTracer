/*
	An old integrator.
*/

//#pragma once
//#include "./../integrator.h"
//
//#include "./../utility.h"
//#include "./../material.h"
//#include "./../pdf.h"
//
//
//#include <memory>
//
//class SampleIntegrator : Integrator {
//public:
//	vec3 Li(const ray& r, hitable *world, int depth);
//
//};
//
//vec3 SampleIntegrator::Li(const ray& r, hitable *world, int depth) {
//	hit_record rec;
//
//	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
//		ray scattered;
//		vec3 attenuation;
//		vec3 emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
//		float pdf;
//		float cos_pdf;
//		float light_pdf;
//		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf)) {
//
////			hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, 0);
//			std::unique_ptr<hitable> light_shape(new xz_rect(213, 343, 227, 332, 554, 0));
//			hitable_pdf p0(light_shape.get(), rec.p);
//			cosine_pdf p1(rec.normal);
//			mixture_pdf p(&p0, &p1);
//			
//			scattered = ray(rec.p, p.generate());
//			pdf = p.value(scattered.direction());
//	//		vec3 dir = p1.generate();
//	//		scattered = ray(rec.p, dir);
//	//		pdf = p1.value(scattered.direction());
//
//			//if (depth == 0) {
//			//	lanlog::log_info("every depth = 1 scattered");
//			//	lanlog::log_info(output_vec3(dir));
//			//	lanlog::log_info(to_string(pdf));
//			//}
//
//			return emitted + attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered) / pdf * Li(scattered, world, depth + 1);
//		}
//		else {
//			return emitted;
//		}
//
//		//vec3 target = rec.p + rec.normal + random_in_unit_sphere();
//		//return 0.5*color(ray(rec.p,target-rec.p),world,depth+1);
//	}
//	else {
//		/*vec3 unit_direction = unit_vector(r.direction());
//		float t = 0.5*(unit_direction.y() + 1.0);
//		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);*/
//		return vec3(0, 0, 0);
//	}
//}
