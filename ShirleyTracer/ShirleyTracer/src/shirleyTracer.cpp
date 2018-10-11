
#include "utility.h"
#include "lanlog.h"
#include "Bitmap.h"
#include "hitable.h"
#include "camera.h"

#include "World.h"
#include ".\integrator\RayCastIntegrator.h"
#include "integrator/AreaLightIntegrator.h"
#include "integrator/WhittedIntegrator.h"

#include "materials/matte_material.h"
#include "materials/Phong.h"
#include "materials/SV_Matte.h"
#include "materials/Reflective.h"

#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/openCylinder.h"
#include "shapes/Compound.h"
#include "shapes/SolidCylinder.h"

#include "shapes/Plane.h"
#include "shapes/Instance.h"
#include "shapes/triangle.h"
#include "shapes/Box.h"
#include "shapes/Rectangle.h"
#include "shapes/accelerator/Grid.h"

#include "extern/Image.h"

#include "texture/ImageTexture.h"
#include "texture/SphericalMap.h"
#include "texture/Checker3D.h"
#include "texture/PlaneChecker.h"

#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

#include "sampler/Jittered.h"

#include <iostream>

#include "plyparser.h"

World* build() {
	
	int nx = 600;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(75, 40, 100);
	vec3 lookat(-10, 39, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 360;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new WhittedIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	w->max_depth = 10;
	PointLight* light_ptr = new PointLight(3.0,white,vec3(150,150,0));
	light_ptr->set_shadows(true);
	w->add_light(light_ptr);

	// yellow-green reflective sphere

	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.25);
	reflective_ptr1->set_kd(0.5);
	reflective_ptr1->set_cd(0.75, 0.75, 0);    	// yellow
	reflective_ptr1->set_ks(0.15);
	reflective_ptr1->set_exp(100.0);
	reflective_ptr1->set_kr(0.75);
	reflective_ptr1->set_cr(white); 			// default color

	float radius = 23.0;
	sphere* sphere_ptr1 = new sphere(vec3(38, radius, -25), radius);
	sphere_ptr1->set_material(reflective_ptr1);
	w->add_object(sphere_ptr1);

	// orange non-reflective sphere

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.45);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(0.75, 0.25, 0);   // orange

	sphere* sphere_ptr2 = new sphere(vec3(-7, 10, 42), 20);
	sphere_ptr2->set_material(matte_ptr1);
	w->add_object(sphere_ptr2);


	// sphere on top of box

	Reflective* reflective_ptr2 = new Reflective;
	reflective_ptr2->set_ka(0.35);
	reflective_ptr2->set_kd(0.75);
	reflective_ptr2->set_cd(black);
	reflective_ptr2->set_ks(0.0);		// default value
	reflective_ptr2->set_exp(1.0);		// default value, but irrelevant in this case
	reflective_ptr2->set_kr(0.75);
	reflective_ptr2->set_cr(white);

	sphere* sphere_ptr3 = new sphere(vec3(-30, 59, 35), 20);
	sphere_ptr3->set_material(reflective_ptr2);
	w->add_object(sphere_ptr3);


	// cylinder

	Reflective* reflective_ptr3 = new Reflective;
	reflective_ptr3->set_ka(0.35);
	reflective_ptr3->set_kd(0.5);
	reflective_ptr3->set_cd(0, 0.5, 0.75);   // cyan
	reflective_ptr3->set_ks(0.2);
	reflective_ptr3->set_exp(100.0);
	reflective_ptr3->set_kr(0.75);
	reflective_ptr3->set_cr(white);

	double bottom = 0.0;
	double top = 85;
	double cylinder_radius = 22;
	SolidCylinder* cylinder_ptr = new SolidCylinder(bottom, top, cylinder_radius);
	cylinder_ptr->set_material(reflective_ptr3);
	w->add_object(cylinder_ptr);


	// box

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.15);
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(0.75, 1.0, 0.75);   // light green

	Box* box_ptr = new Box(vec3(-35, 0, -110), vec3(-25, 60, 65));
	box_ptr->set_material(matte_ptr2);
	w->add_object(box_ptr);



	// ground plane

	PlaneChecker* checker_ptr = new PlaneChecker;
	checker_ptr->set_size(20.0);
	checker_ptr->set_outline_width(2.0);
	checker_ptr->set_color1(white);
	checker_ptr->set_color2(white);
	checker_ptr->set_outline_color(black);

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.30);
	sv_matte_ptr->set_kd(0.9);
	sv_matte_ptr->set_cd(checker_ptr);

	Plane* plane_ptr = new Plane(vec3(0), vec3(0, 1, 0));
	plane_ptr->set_material(sv_matte_ptr);
	w->add_object(plane_ptr);

	w->background_color = vec3(0.15);

	return w;
}


int main() {

	lanlog::initLogging();

	//	LOG(WARNING) << "This is a warning message";
		//LOG(ERROR) << "This is an error message";
		//LOG(FATAL) << "This is a fatal message";
	
	
		World* w = build();
		Jittered* sampler = new Jittered(w->ns);

		std::auto_ptr<Bitmap> pic(new Bitmap(w->nx, w->ny));
		for (int j = w->ny - 1; j >= 0; j--) {
	
			if (j % (w->ny / 10) == 0)
				std::cout << "-";
	
			for (int i = 0; i < w->nx; i++) {
	
	
				vec3 col;
				for (int s = 0; s < w->ns; s++) {
					/*float u = (float(i) + randd()) / float(w->nx);
					float v = (float(j) + randd()) / float(w->ny);*/
					vec2 sampled = sampler->sample_unit_square();
					float u = (float(i) + sampled[0]) / float(w->nx);
					float v = (float(j) + sampled[1]) / float(w->ny);
					ray r = w->camera_ptr->get_ray(u, v);
	
					//		vec3 p = r.point_at_parameter(2.0);
					col += w->integrator_ptr->Li(r,0);
				
				}
	
				col /= w->ns;
	
				for(int idx = 0;idx <3;idx++)
					col[idx] = col[idx] < 0 ? 0 : col[idx];
			//	col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

				// do some out of gamut processing
				if (col[0] > 1.0 || col[1] > 1.0 || col[2] > 1.0) {
					float maxfre = max(col[0], max(col[1], col[2]));
					for (int j = 0; j < 3; j++)
						col[j] /= maxfre;
				}

				int ir = int(255.99*col[0]);
				int ig = int(255.99*col[1]);
				int ib = int(255.99*col[2]);
				pic->SetPixel(i, j, (ir << 16) | (ig << 8) | ib);
			}
		}
	
	
		std::cout << "\n" << "Rendering done";
		pic->SaveBMP("./results/24-6.bmp");
	
	
		lanlog::endLogging();

	//	while (1);
}

