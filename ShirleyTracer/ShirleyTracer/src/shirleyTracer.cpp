#define _CRT_SECURE_NO_WARNINGS

#include "utility.h"
#include "lanlog.h"
#include "Bitmap.h"
#include "hitable.h"
#include "camera.h"
#include "Camera/Pinhole.h"

#include "World.h"
#include ".\integrator\RayCastIntegrator.h"
#include "integrator/AreaLightIntegrator.h"
#include "integrator/WhittedIntegrator.h"
#include "integrator/PathIntegrator.h"
#include "integrator/GlobalPathIntegrator.h"

#include "materials/matte_material.h"
#include "materials/Phong.h"
#include "materials/SV_Matte.h"
#include "materials/Reflective.h"
#include "materials/GlossyReflective.h"
#include "materials/Transparent.h"

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
#include "shapes/accelerator/BVH.h"

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
#include "parsing.h"


World* build() {

	int nx = 600;
	int ny = 600;
	int ns = 1;

	World* w = new World;
	vec3 lookfrom(-5, 5.5, 35.0);
	vec3 lookat(1.25, 3.5, 0.0);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 0.0;
	float distance = 1200;

	// an temporary method to deal with it
	float vfov = 2 * atan2(200, distance) * 180 / M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new WhittedIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->background_color = vec3(0.0,0.3,0.25);
	w->ambient_ptr = new Ambient_Light(0.25, vec3(1.0, 1.0, 1.0));

	w->max_depth = 5;


	Transparent* glass_ptr = new Transparent;
	glass_ptr->set_ks(0.2);
	glass_ptr->set_exp(2000.0);
	glass_ptr->set_ior(1.5);
	glass_ptr->set_kr(0.1);
	glass_ptr->set_kt(0.9);

	Reflective*	reflective_ptr = new Reflective;
	reflective_ptr->set_ka(0.3);
	reflective_ptr->set_kd(0.3);
	reflective_ptr->set_cd(red);
	reflective_ptr->set_ks(0.2);
	reflective_ptr->set_exp(2000.0);
	reflective_ptr->set_kr(0.25);

	//Phong* phong_ptr = new Phong;
	//phong_ptr->set_ka(0.2);
	//phong_ptr->set_kd(0.95);
	//phong_ptr->set_cd(1, 0.6, 0);   // orange
	//phong_ptr->set_ks(0.5);
	//phong_ptr->set_exp(20);

	//sphere* sphere_ptr2 = new sphere(vec3(4, 4, -6), 3);
	//sphere_ptr2->set_material(reflective_ptr);
	//w->add_object(sphere_ptr2);

//	Mesh* m = new Mesh;
//	m->read_file("./geometry/bunny.ply");
////	m->set_mesh_material(glass_ptr);
//	Grid* grid_ptr = new Grid(m);
////	w->add_object(grid_ptr);
//	// point light 
//
//	Instance* big_bunny_ptr = new Instance(grid_ptr);
//	big_bunny_ptr->scale(45.0);
//	big_bunny_ptr->translate(1.7, -1.5, 0.0);
//	big_bunny_ptr->set_material(reflective_ptr);
//	big_bunny_ptr->name = "bunny";
//	w->add_object(big_bunny_ptr);

	Mesh* m = new Mesh;
	m->read_file("./geometry/bunny.ply");
	m->set_mesh_material(reflective_ptr);
	vector<hitable*> tar;
	for (int j = 0; j < m->num_triangles; j++) {
		int a = m->vertex_faces[j][0];
		int b = m->vertex_faces[j][1];
		int c = m->vertex_faces[j][2];

		MeshTriangle* obj_ptr = new MeshTriangle(m, a, b, c,j);
		tar.push_back(obj_ptr);
	}

	BVH* bvh_ptr = new BVH(tar,0,tar.size()-1);


	Instance* big_bunny_ptr = new Instance(bvh_ptr);
	big_bunny_ptr->scale(45.0);
	big_bunny_ptr->translate(1.7, -1.5, 0.0);
	big_bunny_ptr->set_material(reflective_ptr);
//	big_bunny_ptr->name = "bunny";
	w->add_object(big_bunny_ptr);

	// point light 

	PointLight* light_ptr1 = new PointLight(4.0,vec3(1.0,1.0,1.0), vec3(40, 50, 0));
	light_ptr1->set_shadows(false);
	w->add_light(light_ptr1);

	PointLight* light_ptr2 = new PointLight(4.0, vec3(1.0, 1.0, 1.0), vec3(-10, 20, 10));
	light_ptr2->set_shadows(false);
	w->add_light(light_ptr2);
	// directional light 

	DirectionLight* light_ptr3 = new DirectionLight;
	light_ptr3->set_direction(vec3(-1, 0, 0));
	light_ptr3->scale_radiance(4.0);
	light_ptr3->set_shadows(false);
	w->add_light(light_ptr3);


	Checker3D* checker_ptr = new Checker3D;
	checker_ptr->set_size(4);
	checker_ptr->set_color1(0.75);
	checker_ptr->set_color2(white);

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(shared_ptr<Texture>(new ConstantColor(0.5)));
	sv_matte_ptr->set_kd(shared_ptr<Texture>(new ConstantColor(0.35)));
	sv_matte_ptr->set_cd(shared_ptr<Texture>(checker_ptr));

	// rectangle

	vec3 p0(-20, 0, -100);
	vec3 a(0, 0, 120);
	vec3 b(40, 0, 0);
	vec3 normal(0, 1, 0);

	Rectangle* rectangle_ptr = new Rectangle(p0, a, b,normal);
	rectangle_ptr->set_material(sv_matte_ptr);
	w->add_object(rectangle_ptr);


	return w;
}


int main() {


	World* ptr = new World;
	ptr->integrator_ptr = new WhittedIntegrator(ptr);
	FILE* f;
	fopen_s(&f, "./kitchen/kitchen.aff","r");
	viParseFile(f, ptr);
	fclose(f);

	Animation* anim;
	const char* literalstr = "camera";
	char* target_str = new char[strlen(literalstr)+1];
	strcpy(target_str, literalstr);
	//anim = FindAnimation(target_str, mAnimations);

	//int vis;
	//double trans[3];
	//double rot[4];
	//double scale[3];
	//trans[0] = trans[1] = trans[2] = 0;
	//rot[0] = 1; rot[1] = rot[2] = rot[3] = 0;
	//scale[0] = scale[1] = scale[2] = 1;
	//
	//hitable* empty_ptr = 0;
	//Instance* camera_inst = new Instance(empty_ptr);

//	double time = 15.5;
	double time = 0.0;

	int gotPosition=0;
	double viewPos[3];
	int gotDirection=0;
	double viewDir[3];
	double viewUp[3];
	GetCamera(mAnimations, time, &gotPosition,viewPos,&gotDirection,viewDir,viewUp);
	vec3 r_eye = mViewParams->from;
	vec3 r_lookat = mViewParams->at;
	vec3 up = vec3(viewUp[0], viewUp[1], viewUp[2]);
	if(gotPosition)
		r_eye = vec3(viewPos[0], viewPos[1], viewPos[2]);
	if (gotDirection) {
		r_lookat = vec3(r_eye.x() + viewDir[0], r_eye.y() + viewDir[1], r_eye.z() + viewDir[2]);
	}

	float dist_to_focus = (r_eye - r_lookat).length();
	float aperture = 0.0;
	vec3 dir(viewDir[0], viewDir[1], viewDir[2]);
	

//	std::cout << point.x() << " " << point.y() << " " << endl;
//	assert(dot(dir, up) == 0);
	//float distance = 2000;
	float distance = 500;
	
	std::cout << distance << std::endl;

	// an temporary method to deal with it
	float vfov = 2 * atan2(200, distance) * 180 / M_PI;

	//// an temporary method to deal with it
	//float vfov = 2 * atan2(200, distance) * 180 / M_PI;
	ptr->camera_ptr = new Camera(r_eye, r_lookat, up, vfov, float(mViewParams->resx) / float(mViewParams->resy), aperture, dist_to_focus);


	ptr->integrator_ptr = new WhittedIntegrator(ptr);
	ptr->nx = 800;
	ptr->ny = 600;
	ptr->ns = 16;

//	ptr->ambient_ptr = new Ambient_Light(0.4, vec3(1.0, 1.0, 1.0));

	ptr->max_depth = 5;


	lanlog::initLogging();
	manual_timer read_timer;

	read_timer.start();

	//	LOG(WARNING) << "This is a warning message";
		//LOG(ERROR) << "This is an error message";
		//LOG(FATAL) << "This is a fatal message";
	
	
		World* w = ptr;
//	World* w = build();
		MultiJittered* sampler = new MultiJittered(w->ns);

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
	
					//vec3 dir = w->ncam_ptr->get_direction(u, v);
					//
					//ray r = ray(w->ncam_ptr->get_eye(), dir);

					col += w->integrator_ptr->Li(r,0);
				
				}
	
				col /= w->ns;
	
			/*	for(int idx = 0;idx <3;idx++)
					col[idx] = col[idx] < 0 ? 0 : col[idx];
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));*/

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
	
	
		std::cout << "\n" << "Rendering done\n";
		pic->SaveBMP("./results/bart_test.bmp");
	

		read_timer.stop();
		lanlog::endLogging();

		std::cout << read_timer.get()/1000.0 << "s consumed" << endl;
		while (1);
}

