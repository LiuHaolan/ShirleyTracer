
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

#include "sampler/Jittered.h"

#include <iostream>

#include "plyparser.h"

World* build() {
	
	int nx = 600;
	int ny = 600;
	int ns = 25;

	World* w = new World;
	vec3 lookfrom(-150, 75, 500);
	vec3 lookat(-6, 50, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 2000;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new AreaLightIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	w->max_depth = 1;
	PointLight* light_ptr = new PointLight(2.0,white,vec3(250,500,250));
	light_ptr->set_shadows(false);
	w->add_light(light_ptr);

	// yellow-green reflective sphere

//	float exp = 1.0;  		// for Figure 25.30(a)
//	float exp = 10.0;  		// for Figure 25.30(b)
//	float exp = 100.0;  	// for Figure 25.30(c)
//	float exp = 1000.0;  	// for Figure 25.30(d)
	float exp = 10000.0;  	// for Figure 25.30(e)
//	float exp = 100000.0;  	// for Figure 25.30(f)

	GlossyReflective* glossy_ptr = new GlossyReflective;
	glossy_ptr->set_samples(ns, exp);
	glossy_ptr->set_ka(0.0);
	glossy_ptr->set_kd(0.0);
	glossy_ptr->set_ks(0.0);	// 0.0 lhl
	glossy_ptr->set_exp(exp);
	glossy_ptr->set_cd(1.0, 1.0, 0.3);
	glossy_ptr->set_kr(0.9);
	glossy_ptr->set_exponent(exp);
	glossy_ptr->set_cr(vec3(1.0, 1.0, 0.3));  // lemon

	sphere* sphere_ptr1 = new sphere(vec3(-6, 55, 0), 40);
	sphere_ptr1->set_material(glossy_ptr);
	w->add_object(sphere_ptr1);

	//cylinder

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0.15);
	matte_ptr->set_kd(0.75);
	matte_ptr->set_cd(0.5, 1.0, 0.5);   // green

	double bottom = -100;
	double top = 15;
	double radius = 30;
	Instance* cylinder_ptr = new Instance(new SolidCylinder(bottom, top, radius));
	cylinder_ptr->translate(-6, 0, 0);
	cylinder_ptr->set_material(matte_ptr);
	w->add_object(cylinder_ptr);

	Image* image_ptr = new Image;
	image_ptr->read_file("uffizi_probe_small.ppm");  // for testing 
	LightProbe* light_probe_ptr = new LightProbe;
	light_probe_ptr->set_map_type(light_prob);
	ImageTexture* image_texture_ptr = new ImageTexture(image_ptr);
	image_texture_ptr->set_mapping(light_probe_ptr);

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(1);
	sv_matte_ptr->set_kd(0.85);
	sv_matte_ptr->set_cd(image_texture_ptr);

	sphere* unit_sphere_ptr = new sphere;
//	unit_sphere_ptr->set_shadows(false);

	Instance* sphere_ptr2 = new Instance(unit_sphere_ptr);
	sphere_ptr2->scale(1000000.0);
	sphere_ptr2->set_material(sv_matte_ptr);
	sphere_ptr2->transform_texture(true);

	w->add_object(sphere_ptr2);

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

