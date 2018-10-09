
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
#include "shapes/Plane.h"
#include "shapes/Instance.h"
#include "shapes/triangle.h"
#include "shapes/Box.h"
#include "shapes/Rectangle.h"

#include "extern/Image.h"
#include "texture/ImageTexture.h"
#include "texture/SphericalMap.h"
#include "texture/Checker3D.h"

#include "lights/DirectionLight.h"
#include "lights/PointLight.h"
#include "lights/AreaLight.h"

#include "sampler/Jittered.h"

#include <iostream>

World* build() {
	
	int nx = 600;
	int ny = 600;
	int ns = 1;

	World* w = new World;
	vec3 lookfrom(-20, 10, 20);
	vec3 lookat(0, 2, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 800;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new AreaLightIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(1.0,vec3(1.0,1.0,1.0));

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->scale_radiance(40.0);
	emissive_ptr->set_ce(white);

	// define a rectangle for the rectangular light

	float width = 4.0;				// for Figure 18.4(a) & (b)
	float height = 4.0;
	//	float width = 2.0;				// for Figure 18.4(c)
	//	float height = 2.0;
	vec3 center(0.0, 7.0, -7.0);	// center of each area light (rectangular, disk, and spherical)
	vec3 p0(-0.5 * width, center.y() - 0.5 * height, center.z());
	vec3 a(width, 0.0, 0.0);
	vec3 b(0.0, height, 0.0);
	vec3 normal(0, 0, 1);

	Sampler* sampler_ptr = new Jittered(ns);
	
	Rectangle* rectangle_ptr = new Rectangle(p0, a, b, normal);
	rectangle_ptr->set_material(emissive_ptr);
	rectangle_ptr->set_sampler(sampler_ptr);
//	rectangle_ptr->set_shadows(false);
	w->add_object(rectangle_ptr);

	AreaLight* area_light_ptr = new AreaLight;
	area_light_ptr->set_object(rectangle_ptr);
	area_light_ptr->set_shadows(false);
	w->add_light(area_light_ptr);

	// Four axis aligned boxes

	float box_width = 1.0; 		// x dimension
	float box_depth = 1.0; 		// z dimension
	float box_height = 4.5; 		// y dimension
	float gap = 3.0;

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.25);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(0.4, 0.7, 0.4);     // green

	Box* box_ptr0 = new Box(vec3(-1.5 * gap - 2.0 * box_width, 0.0, -0.5 * box_depth),
		vec3(-1.5 * gap - box_width, box_height, 0.5 * box_depth));
	box_ptr0->set_material(matte_ptr1);
	w->add_object(box_ptr0);

	Box* box_ptr1 = new Box(vec3(-0.5 * gap - box_width, 0.0, -0.5 * box_depth),
		vec3(-0.5 * gap, box_height, 0.5 * box_depth));
	box_ptr1->set_material(matte_ptr1);
	w->add_object(box_ptr1);

	Box* box_ptr2 = new Box(vec3(0.5 * gap, 0.0, -0.5 * box_depth),
		vec3(0.5 * gap + box_width, box_height, 0.5 * box_depth));
	box_ptr2->set_material(matte_ptr1);
	w->add_object(box_ptr2);

	Box* box_ptr3 = new Box(vec3(1.5 * gap + box_width, 0.0, -0.5 * box_depth),
		vec3(1.5 * gap + 2.0 * box_width, box_height, 0.5 * box_depth));
	box_ptr3->set_material(matte_ptr1);
	w->add_object(box_ptr3);


	// ground plane

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.1);
	matte_ptr2->set_kd(0.90);
	matte_ptr2->set_cd(white);

	Plane* plane_ptr = new Plane(vec3(0.0), vec3(0, 1, 0));
	plane_ptr->set_material(matte_ptr2);
	w->add_object(plane_ptr);

	w->background_color = vec3(0.);

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
		pic->SaveBMP("./results/4.08(b).bmp");
	
	
		lanlog::endLogging();
}

