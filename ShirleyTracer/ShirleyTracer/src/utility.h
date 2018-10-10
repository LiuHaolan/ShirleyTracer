#pragma once

#define M_PI 3.1415926
#define INV_PI 0.3183099
#define MAXFLOAT FLT_MAX

#include "vec3.h"
#include <random>
#include <string>

#include "Bitmap.h"

using namespace std;

const static vec3 white = vec3(1.0, 1.0, 1.0);
const static vec3 black = vec3(0.0, 0.0, 0.0);

static std::default_random_engine generator;
static std::uniform_real_distribution<float> distr(0.0, 1.0);

std::string output_vec3(vec3& c);

// the function to return 0-1 results
float randd();

vec3 random_in_unit_sphere();

vec3 random_in_unit_disk();

void get_sphere_uv(const vec3& p, float &u, float&v);

//// debug some pixel to a picture
//void print_to_png(std::string filename,int hres,int vres) {
//	// write to a file to validate that the file reading API is working well
//	std::auto_ptr<Bitmap> pica(new Bitmap(hres, vres));
//	for(int i = 0;i<hres;i++)
//		for (int j = 0; j < vres; j++) {
//			vec3 col = image_ptr->get_color(vres - j, i);
//
//			int ir = int(255.99*col[0]);
//			int ig = int(255.99*col[1]);
//			int ib = int(255.99*col[2]);
//			pica->SetPixel(i, j, (ir << 16) | (ig << 8) | ib);
//		}
//	std::string fullname = "./LOG/" + filename;
//	pica->SaveBMP(fullname.c_str());
//}

inline float clamp(float x, float min, float max) {
	return (x < min ? min : ((x > max) ? max : x));
};