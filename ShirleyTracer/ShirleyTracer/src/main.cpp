#include <iostream>
#include "Bitmap.h"
#include <memory>
//#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "material.h"

#include "camera.h"
#include <random>

#include <float.h>
#define MAXFLOAT FLT_MAX

#pragma once
#include "utility.h"
#include <ctime>
#include <iostream>
#include <string>

#define PI 3.14159265
//static int ss = 0;



vec3 color(const ray& r, hitable *world, int depth) {

	hit_record rec;

	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}

		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p,target-rec.p),world,depth+1);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian_material(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = randd();
			vec3 center(a + 0.9*randd(), 0.2, b + 0.9*randd());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian_material(vec3(randd()*randd(), randd()*randd(), randd()*randd())));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal_material(vec3(0.5*(1 + randd()), 0.5*(1 + randd()), 0.5*(1 + randd())), 0.5*randd()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian_material(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal_material(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

int main() {

	int nx = 800;
	int ny = 400;
	int ns = 100;

	std::auto_ptr<Bitmap> pic(new Bitmap(nx, ny));

	//vec3 lower_left_corner(-2.0, -1.0, -1.0);
	//vec3 horizontal(4.0, 0.0, 0.0);
	//vec3 vertical(0.0, 2.0, 0.0);
	//vec3 origin(0.0, 0.0,0.0);
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	Camera c(lookfrom,lookat,vec3(0,1,0),20,float(nx)/float(ny),aperture,dist_to_focus);

	/*hitable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5,new lambertian_material(vec3(0.1,0.2,0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian_material(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal_material(vec3(0.8, 0.6, 0.2),0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5); */
	hitable *world = random_scene();
	for (int j = ny - 1; j >= 0; j--) {

		if (j % (ny / 10) == 0)
			std::cout << "-";

		for (int i = 0; i < nx; i++) {


			vec3 col;
			for (int s = 0; s < ns; s++) {
				float u = (float(i) + randd()) / float(nx);
				float v = (float(j) + randd()) / float(ny);
				ray r = c.get_ray(u, v);


				//		vec3 p = r.point_at_parameter(2.0);
				col += color(r, world,0);

			}

			col /= ns;

			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			pic->SetPixel(i, j, (ir << 16) | (ig << 8) | ib);
		}
	}

	std::cout << "\n" << "Rendering done";
	pic->SaveBMP("./results/cover_.bmp");

}