
#include "utility.h"
#include "lanlog.h"
#include "Bitmap.h"
#include "hitable.h"
#include "camera.h"

#include "World.h"
#include ".\integrator\RayCastIntegrator.h"
#include "materials/matte_material.h"
#include "materials/Phong.h"

#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/Plane.h"
#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include <iostream>

World* build() {
	
	int nx = 600;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(7.5, 4.0, 10);
	vec3 lookat(-1, 3.7, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float vfov = 60.0;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new RayCastIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	DirectionLight* light_ptr = new DirectionLight;
	light_ptr->set_direction(vec3(15, 15, 2.5));
	light_ptr->scale_radiance(2.0);
	w->add_light(light_ptr);

//	PointLight* light_ptr2 = new PointLight(2.0,vec3(15,15,2.5),vec3(1.0,1.0,1.0));

	Phong* phong_ptr1 = new Phong;
	phong_ptr1->set_ka(0.25);
	phong_ptr1->set_kd(0.75);
	phong_ptr1->set_cd(0.75, 0.75, 0);  	// dark yellow
	phong_ptr1->set_ks(0.25);
	phong_ptr1->set_exp(50);

	Phong* phong_ptr2 = new Phong;
	phong_ptr2->set_ka(0.45);
	phong_ptr2->set_kd(0.75);
	phong_ptr2->set_cd(0.75, 0.25, 0);   	// orange
	phong_ptr2->set_ks(0.25);
	phong_ptr2->set_exp(500);

	Phong* phong_ptr3 = new Phong;
	phong_ptr3->set_ka(0.4);
	phong_ptr3->set_kd(0.75);
	phong_ptr3->set_cd(1, 0.5, 1);			// mauve
	phong_ptr3->set_ks(0.25);
	phong_ptr3->set_exp(4);

	Phong* phong_ptr4 = new Phong;
	phong_ptr4->set_ka(0.15);
	phong_ptr4->set_kd(0.5);
	phong_ptr4->set_cd(0.75, 1.0, 0.75);   	// light green
	phong_ptr4->set_ks(0.5);
	phong_ptr4->set_exp(3);

	Matte* matte_ptr5 = new Matte;
	matte_ptr5->set_ka(0.20);
	matte_ptr5->set_kd(0.97);
	matte_ptr5->set_cd(1.0);
	// cylinder

	float bottom = 0.0;
	float top = 8.5;
	float radius = 2.2;
	cylinder* cylinder_ptr = new cylinder(bottom, top, radius);
	cylinder_ptr->set_material(phong_ptr3);
	w->add_object(cylinder_ptr);

	sphere* sphere_ptr1 = new sphere(vec3(3.85, 2.3, -2.55), 2.3, phong_ptr1);
	w->add_object(sphere_ptr1);

	sphere* sphere_ptr2 = new sphere(vec3(-0.7, 1, 4.2), 2, phong_ptr2);
	w->add_object(sphere_ptr2);


	float a = 0.75;
	w->background_color = vec3(0.0, 0.3 * a, 0.25 * a);

	return w;
}

int main() {

	lanlog::initLogging();
	
	//	LOG(WARNING) << "This is a warning message";
		//LOG(ERROR) << "This is an error message";
		//LOG(FATAL) << "This is a fatal message";
	


	
		World* w = build();

		std::auto_ptr<Bitmap> pic(new Bitmap(w->nx, w->ny));
		for (int j = w->ny - 1; j >= 0; j--) {
	
			if (j % (w->ny / 10) == 0)
				std::cout << "-";
	
			for (int i = 0; i < w->nx; i++) {
	
	
				vec3 col;
				for (int s = 0; s < w->ns; s++) {
					float u = (float(i) + randd()) / float(w->nx);
					float v = (float(j) + randd()) / float(w->ny);
					ray r = w->camera_ptr->get_ray(u, v);
	
					//		vec3 p = r.point_at_parameter(2.0);
					col += w->integrator_ptr->Li(r,0);
				
				}
	
				col /= w->ns;
	
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

				//// do some out of gamut processing
				//if (col[0] > 1.0 || col[1] > 1.0 || col[2] > 1.0) {
				//	float maxfre = max(col[0], max(col[1], col[2]));
				//	for (int j = 0; j < 3; j++)
				//		col[j] /= maxfre;
				//}

				int ir = int(255.99*col[0]);
				int ig = int(255.99*col[1]);
				int ib = int(255.99*col[2]);
				pic->SetPixel(i, j, (ir << 16) | (ig << 8) | ib);
			}
		}
	
	
		std::cout << "\n" << "Rendering done";
		pic->SaveBMP("./results/15-8.bmp");
	
	
		lanlog::endLogging();
}

