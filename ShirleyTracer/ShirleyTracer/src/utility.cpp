#include "utility.h"



std::string output_vec3(vec3& c) {
	std::string s = "";
	s += to_string(c.x()) + " " + to_string(c.y()) + " " + to_string(c.z());

	return s;
}

// the function to return 0-1 results
float randd() {
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

