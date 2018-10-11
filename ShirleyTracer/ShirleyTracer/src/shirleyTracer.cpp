
#include "utility.h"
#include "lanlog.h"
#include "Bitmap.h"
#include "hitable.h"
#include "camera.h"

#include "World.h"
#include ".\integrator\RayCastIntegrator.h"
#include "integrator/AreaLightIntegrator.h"

#include "materials/matte_material.h"
#include "materials/Phong.h"
#include "materials/SV_Matte.h"

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

#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

#include "sampler/Jittered.h"

#include <iostream>

#include "plyparser.h"

World* build() {
	
	int nx = 400;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(0, 0, 20);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 3600;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new RayCastIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.25,vec3(1.0,1.0,1.0));

	DirectionLight* light_ptr = new DirectionLight;
	light_ptr->set_direction(vec3(-10, 20, 20));
	light_ptr->scale_radiance(3.0);
	light_ptr->set_shadows(true);
	w->add_light(light_ptr);

	int num_spheres = 100;			// for Figure 22.11(a)
//	int num_spheres = 10000;		// for Figure 22.11(b)
//	int num_spheres = 100000;		// for Figure 22.11(c)
//	int num_spheres = 1000000;		// for Figure 22.11(d)			

	float volume = 0.1 / num_spheres;
	float radius = pow(0.75 * volume / 3.14159, 0.333333);

	//set_rand_seed(15);

	Grid* grid_ptr = new Grid;

	for (int j = 0; j < num_spheres; j++) {
		Matte* matte_ptr = new Matte;
		matte_ptr->set_ka(0.25);
		matte_ptr->set_kd(0.75);
		matte_ptr->set_cd(randd(), randd(), randd());

		sphere*	sphere_ptr = new sphere(vec3(1.0 - 2.0 * randd(),
			1.0 - 2.0 * randd(),
			1.0 - 2.0 * randd()), radius, matte_ptr);

//		sphere_ptr->set_material(matte_ptr);
		grid_ptr->add_objects(sphere_ptr);
	}

	grid_ptr->setup_cells();
	w->add_object(grid_ptr);

	w->background_color = vec3(0.);

	return w;
}


int main() {

	

	////read_ply_file("./geometry/dragon.ply");
	//while (1);
	//return 0;

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
		pic->SaveBMP("./results/23-9.bmp");
	
	
		lanlog::endLogging();

	//	while (1);
}

