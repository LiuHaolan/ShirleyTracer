
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
#include "shapes/Instance.h"

#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include <iostream>

World* build() {
	
	int nx = 400;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(100, 0, 100);
	vec3 lookat(0, 1, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 6000;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new RayCastIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	PointLight* light_ptr2 = new PointLight(3.0,vec3(1.0,1.0,1.0), vec3(50, 50, 1));
	w->add_light(light_ptr2);

	Phong* phong_ptr = new Phong;
	phong_ptr->set_cd(0.75);
	phong_ptr->set_ka(0.25);
	phong_ptr->set_kd(0.8);
	phong_ptr->set_ks(0.15);
	phong_ptr->set_exp(50.0);

	Instance* ellipsoid_ptr = new Instance(new sphere);
	ellipsoid_ptr->set_material(phong_ptr);
	ellipsoid_ptr->scale(2, 3, 1);
	ellipsoid_ptr->rotate_x(-45);
	ellipsoid_ptr->translate(0, 1, 0);
	w->add_object(ellipsoid_ptr);
	sphere* sphere_ptr = new sphere;
	sphere_ptr->set_material(phong_ptr);
	w->add_object(sphere_ptr);


	w->background_color = vec3(0.0, 0, 0);

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

