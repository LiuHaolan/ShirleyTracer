
#include "utility.h"
#include "lanlog.h"
#include "Bitmap.h"
#include "hitable.h"
#include "camera.h"

#include "World.h"
#include ".\integrator\RayCastIntegrator.h"
#include "integrator/AreaLightIntegrator.h"
#include "integrator/WhittedIntegrator.h"
#include "integrator/PathIntegrator.h"

#include "materials/matte_material.h"
#include "materials/Phong.h"
#include "materials/SV_Matte.h"
#include "materials/Reflective.h"
#include "materials/GlossyReflective.h"

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
#include "texture/LightProbe.h"

#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

#include "sampler/MultiJittered.h"
#include "sampler/Jittered.h"

#include <iostream>

#include "plyparser.h"

World* build() {

	int nx = 300;
	int ny = 300;
	int ns = 1024;

	World* w = new World;
	vec3 lookfrom(27.6, 27.4, -80.0);
	vec3 lookat(27.6, 27.4, 0.0);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 0.0;
	float distance = 400;

	// an temporary method to deal with it
	float vfov = 2 * atan2(200, distance) * 180 / M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new PathIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.0, vec3(1.0, 1.0, 1.0));

	w->max_depth = 10;
	// the ceiling light - doesn't need samples

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->set_ce(1.0, 0.73, 0.4);
	emissive_ptr->scale_radiance(100);

	vec3 p0;
	vec3 a, b;
	vec3 normal;

	// box dimensions

	double width = 55.28;   	// x direction
	double height = 54.88;  	// y direction
	double depth = 55.92;	// z direction

	p0 = vec3(21.3, height - 0.001, 22.7);
	a = vec3(0.0, 0.0, 10.5);
	b = vec3(13.0, 0.0, 0.0);
	normal = vec3(0.0, -1.0, 0.0);
	Rectangle* light_ptr = new Rectangle(p0, a, b, normal);
	light_ptr->set_material(emissive_ptr);
	w->add_object(light_ptr);


	// left wall

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.0);
	matte_ptr1->set_kd(0.6);
	matte_ptr1->set_cd(0.57, 0.025, 0.025);	 // red
	matte_ptr1->set_sampler(new Jittered(ns));

	p0 = vec3(width, 0.0, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(0.0, height, 0.0);
	normal = vec3(-1.0, 0.0, 0.0);
	Rectangle* left_wall_ptr = new Rectangle(p0, a, b, normal);
	left_wall_ptr->set_material(matte_ptr1);
	w->add_object(left_wall_ptr);


	// right wall

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.0);
	matte_ptr2->set_kd(0.6);
	matte_ptr2->set_cd(0.37, 0.59, 0.2);	 // green   from Photoshop
	matte_ptr2->set_sampler(new Jittered(ns));

	p0 = vec3(0.0, 0.0, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(0.0, height, 0.0);
	normal = vec3(1.0, 0.0, 0.0);
	Rectangle* right_wall_ptr = new Rectangle(p0, a, b, normal);
	right_wall_ptr->set_material(matte_ptr2);
	w->add_object(right_wall_ptr);


	// back wall

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(0.0);
	matte_ptr3->set_kd(0.6);
	matte_ptr3->set_cd(white);
	matte_ptr3->set_sampler(new Jittered(ns));

	p0 = vec3(0.0, 0.0, depth);
	a = vec3(width, 0.0, 0.0);
	b = vec3(0.0, height, 0.0);
	normal = vec3(0.0, 0.0, -1.0);
	Rectangle* back_wall_ptr = new Rectangle(p0, a, b, normal);
	back_wall_ptr->set_material(matte_ptr3);
	w->add_object(back_wall_ptr);


	// floor

	p0 = vec3(0.0, 0.0, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(width, 0.0, 0.0);
	normal = vec3(0.0, 1.0, 0.0);
	Rectangle* floor_ptr = new Rectangle(p0, a, b, normal);
	floor_ptr->set_material(matte_ptr3);
	w->add_object(floor_ptr);


	// ceiling

	p0 = vec3(0.0, height, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(width, 0.0, 0.0);
	normal = vec3(0.0, -1.0, 0.0);
	Rectangle* ceiling_ptr = new Rectangle(p0, a, b, normal);
	ceiling_ptr->set_material(matte_ptr3);
	w->add_object(ceiling_ptr);


	//// the two boxes defined as 5 rectangles each

	//// short box

	//// top

	//p0 = vec3(13.0, 16.5, 6.5);
	//a = vec3(-4.8, 0.0, 16.0);
	//b = vec3(16.0, 0.0, 4.9);
	//normal = vec3(0.0, 1.0, 0.0);
	//Rectangle* short_top_ptr = new Rectangle(p0, a, b, normal);
	//short_top_ptr->set_material(matte_ptr3);
	//w->add_object(short_top_ptr);


	//// side 1

	//p0 = vec3(13.0, 0.0, 6.5);
	//a = vec3(-4.8, 0.0, 16.0);
	//b = vec3(0.0, 16.5, 0.0);
	//Rectangle* short_side_ptr1 = new Rectangle(p0, a, b);
	//short_side_ptr1->set_material(matte_ptr3);
	//w->add_object(short_side_ptr1);


	//// side 2

	//p0 = vec3(8.2, 0.0, 22.5);
	//a = vec3(15.8, 0.0, 4.7);
	//Rectangle* short_side_ptr2 = new Rectangle(p0, a, b);
	//short_side_ptr2->set_material(matte_ptr3);
	//w->add_object(short_side_ptr2);


	//// side 3

	//p0 = vec3(24.2, 0.0, 27.4);
	//a = vec3(4.8, 0.0, -16.0);
	//Rectangle* short_side_ptr3 = new Rectangle(p0, a, b);
	//short_side_ptr3->set_material(matte_ptr3);
	//w->add_object(short_side_ptr3);


	//// side 4

	//p0 = vec3(29.0, 0.0, 11.4);
	//a = vec3(-16.0, 0.0, -4.9);
	//Rectangle* short_side_ptr4 = new Rectangle(p0, a, b);
	//short_side_ptr4->set_material(matte_ptr3);
	//w->add_object(short_side_ptr4);



	//// tall box

	//// top

	//p0 = vec3(42.3, 33.0, 24.7);
	//a = vec3(-15.8, 0.0, 4.9);
	//b = vec3(4.9, 0.0, 15.9);
	//normal = vec3(0.0, 1.0, 0.0);
	//Rectangle* tall_top_ptr = new Rectangle(p0, a, b, normal);
	//tall_top_ptr->set_material(matte_ptr3);
	//w->add_object(tall_top_ptr);


	//// side 1

	//p0 = vec3(42.3, 0.0, 24.7);
	//a = vec3(-15.8, 0.0, 4.9);
	//b = vec3(0.0, 33.0, 0.0);
	//Rectangle* tall_side_ptr1 = new Rectangle(p0, a, b);
	//tall_side_ptr1->set_material(matte_ptr3);
	//w->add_object(tall_side_ptr1);


	//// side 2

	//p0 = vec3(26.5, 0.0, 29.6);
	//a = vec3(4.9, 0.0, 15.9);
	//Rectangle* tall_side_ptr2 = new Rectangle(p0, a, b);
	//tall_side_ptr2->set_material(matte_ptr3);
	//w->add_object(tall_side_ptr2);


	//// side 3

	//p0 = vec3(31.4, 0.0, 45.5);
	//a = vec3(15.8, 0.0, -4.9);
	//Rectangle* tall_side_ptr3 = new Rectangle(p0, a, b);
	//tall_side_ptr3->set_material(matte_ptr3);
	//w->add_object(tall_side_ptr3);


	//// side 4

	//p0 = vec3(47.2, 0.0, 40.6);
	//a = vec3(-4.9, 0.0, -15.9);
	//Rectangle* tall_side_ptr4 = new Rectangle(p0, a, b);
	//tall_side_ptr4->set_material(matte_ptr3);
	//w->add_object(tall_side_ptr4);

	w->background_color = vec3(0.0);

	return w;
}


int main() {

//	// visualize sampler.
//	int num = 100;
//	MultiJittered* s = new MultiJittered(100);
//	s->map_samples_to_hemisphere(1.0);
//	std::auto_ptr<Bitmap> p(new Bitmap(300, 300));
//	vec3 col = white;
//	vec3 back = black;
//	int ir = int(255.99*col[0]);
//	int ig = int(255.99*col[1]);
//	int ib = int(255.99*col[2]);
//	for (int j = 0; j < 300*300; j++) {
//		p->SetPixel(j/300, j%300, (0 << 16) | (0 << 8) | 0);
//	}
//	int cnt = 0;
//	int q_cnt = 0;
//	for (int j = 0; j < num; j++) {
//		vec2 sam = s->sample_unit_square();
//		int x = (int)(sam[0]*300);
//		int y = (int)(sam[1] * 300);
//		p->SetPixel(x, y, (ir << 16) | (ig << 8) | ib);
//		// test hemisphere
////		vec3 sam = s->sample_hemisphere();
//		//int x = (int)(sam[0] * 300);
//		//int y = (int)(sam[2] * 300);
//		//if (y > 150)
//		//	cnt++;
//		//else
//		//	q_cnt++;
//		//p->SetPixel(x, y, (ir << 16) | (ig << 8) | ib);
//	}
//	p->SaveBMP("./results/sample.bmp");
////	std::cout << cnt << " " << q_cnt << std::endl;
////	while (1);
//	return 0;


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
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

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
		pic->SaveBMP("./results/25-8shuffled.bmp");
	
	
		lanlog::endLogging();

	//	while (1);
}

