
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
#include "shapes/triangle.h"

#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include <iostream>

World* build() {
	
	int nx = 600;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(2, 2.5, 15);
	vec3 lookat(3, 2.5, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 700;

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

	DirectionLight* light_ptr1 = new DirectionLight;
	light_ptr1->set_direction(vec3(200, 150, 125));
	light_ptr1->scale_radiance(4.0);
	//	light_ptr1->set_shadows(true);				// for Figure 16.1(b)
	w->add_light(light_ptr1);

	PointLight* light_ptr2 = new PointLight(4.0,vec3(1.0,1.0,1.0), vec3(200, 150, 125));
	w->add_light(light_ptr2);

	PointLight* light_ptr3 = new PointLight(4.0, vec3(1.0, 1.0, 1.0), vec3(-12, 15, 30));
	w->add_light(light_ptr3);

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(0.35);
	matte_ptr3->set_kd(0.50);
	matte_ptr3->set_cd(0, 0.5, 0.5);      // cyan

	Triangle* triangle_ptr1 = new Triangle(vec3(1.5, -0.5, 1.8), 		// front
		vec3(7.5, -0.5, -9.00), 		// back
		vec3(2.35, 5.8, 1.4));		// top									
	triangle_ptr1->set_material(matte_ptr3);
	w->add_object(triangle_ptr1);


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

