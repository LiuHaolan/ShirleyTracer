//#pragma once
//
//#include <iostream>
//#include "Bitmap.h"
//#include <memory>
////#include "vec3.h"
//#include "ray.h"
//#include "hitable_list.h"
//#include "sphere.h"
//#include "xy_rect.h"
//#include "material.h"
//
//#include "camera.h"
//#include <random>
//
//#include <float.h>
//
//
//#pragma once
//#include "utility.h"
//#include "scenefile.h"
//#include "instancing.h"
//
//#include ".\integrator\SampleIntegrator.h"
//
//#include <ctime>
//#include <iostream>
//#include <string>
//
//#define PI 3.14159265
////static int ss = 0;
//#define STB_IMAGE_IMPLEMENTATION
//#include "extern/stb_image.h"
////#include "glog/logging.h"
//#include "lanlog.h"
//
////hitable *random_scene() {
////	int n = 500;
////	hitable **list = new hitable*[n + 1];
//////	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian_material(vec3(0.5, 0.5, 0.5)));
////	
////	texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
////	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian_material(checker));
////
////	int i = 1;
////	for (int a = -11; a < 11; a++) {
////		for (int b = -11; b < 11; b++) {
////			float choose_mat = randd();
////			vec3 center(a + 0.9*randd(), 0.2, b + 0.9*randd());
////			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
////				if (choose_mat < 0.8) {  // diffuse
////					list[i++] = new sphere(center, 0.2, new lambertian_material(new constant_texture(vec3(randd()*randd(), randd()*randd(), randd()*randd()))));
////				}
////				else if (choose_mat < 0.95) { // metal
////					list[i++] = new sphere(center, 0.2,
////						new metal_material(vec3(0.5*(1 + randd()), 0.5*(1 + randd()), 0.5*(1 + randd())), 0.5*randd()));
////				}
////				else {  // glass
////					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
////				}
////			}
////		}
////	}
////
////	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
////	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian_material(new constant_texture(vec3(0.4, 0.2, 0.1))));
////	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal_material(vec3(0.7, 0.6, 0.5), 0.0));
////
////	return new hitable_list(list, i);
////}
////
////hitable *simple_light() {
////	hitable **list = new hitable*[4];
////	int pnx, pny, pnn;
////	unsigned char* tex_data = stbi_load("earthmap1k.jpg", &pnx, &pny, &pnn, 0);
////
////	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian_material(new checker_texture(new constant_texture(vec3(0, 0, 0)),new constant_texture(vec3(1, 1, 1)))));
////	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian_material(new image_texture(tex_data, pnx, pny)));
////	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
////	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
////	return new hitable_list(list, 4);
////}
//
//hitable *cornell_box() {
//	hitable **list = new hitable*[8];
//	int i = 0;
//	material *red = new lambertian_material(new constant_texture(vec3(0.65, 0.05, 0.05)));
//	material *white = new lambertian_material(new constant_texture(vec3(0.73, 0.73, 0.73)));
//	material *green = new lambertian_material(new constant_texture(vec3(0.12, 0.45, 0.15)));
//	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
//	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
//	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
//	list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
//	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
//	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
//	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
//	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white),-18),vec3(130,0,65));
//	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
//
//	return new hitable_list(list, i);
//}
//
//void oldmain() {
//
//	lanlog::initLogging();
//
////	LOG(WARNING) << "This is a warning message";
//	//LOG(ERROR) << "This is an error message";
//	//LOG(FATAL) << "This is a fatal message";
//
//	int nx = 500;
//	int ny = 500;
//	int ns = 100;
//
//	std::auto_ptr<Bitmap> pic(new Bitmap(nx, ny));
//
//	vec3 lookfrom(278, 278, -800);
//	vec3 lookat(278, 278, 0);
//	float dist_to_focus = 10.0;
//	float aperture = 0.0;
//	float vfov = 40.0;
//	Camera c(lookfrom,lookat,vec3(0,1,0),vfov,float(nx)/float(ny),aperture,dist_to_focus);
//
//	hitable *world = cornell_box();
//
//	SampleIntegrator* newton = new SampleIntegrator;
//
//	for (int j = ny - 1; j >= 0; j--) {
//
//		if (j % (ny / 10) == 0)
//			std::cout << "-";
//
//		for (int i = 0; i < nx; i++) {
//
//
//			vec3 col;
//			for (int s = 0; s < ns; s++) {
//				float u = (float(i) + randd()) / float(nx);
//				float v = (float(j) + randd()) / float(ny);
//				ray r = c.get_ray(u, v);
//
//				//		vec3 p = r.point_at_parameter(2.0);
//				col += newton->Li(r, world,0);
//			
//			}
//
//			col /= ns;
//
//			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
//			int ir = int(255.99*col[0]);
//			int ig = int(255.99*col[1]);
//			int ib = int(255.99*col[2]);
//			pic->SetPixel(i, j, (ir << 16) | (ig << 8) | ib);
//		}
//	}
//
//	// using smart pointer to avoid this shit
//	delete newton;
//
//	std::cout << "\n" << "Rendering done";
//	pic->SaveBMP("./results/cornell_box_mixsample.bmp");
//
//
//	lanlog::endLogging();
//}