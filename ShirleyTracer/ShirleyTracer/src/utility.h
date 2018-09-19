#pragma once

#define M_PI 3.1415926
#define MAXFLOAT FLT_MAX

#include <random>
static std::default_random_engine generator;
static std::uniform_real_distribution<float> distr(0.0, 1.0);

// the function to return 0-1 results
float inline randd() {
	//	srand((unsigned)time(0));
	//	return rand()*1.0 / (float)RAND_MAX;

	return distr(generator);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(randd(), randd(), randd()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0*vec3(randd(), randd(), 0) - vec3(1, 1, 0);
	} while (p.squared_length() >= 1.0);
	return p;
}

void get_sphere_uv(const vec3& p, float &u, float&v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}