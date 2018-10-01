#pragma once

#define M_PI 3.1415926
#define INV_PI 0.3183099
#define MAXFLOAT FLT_MAX

#include "vec3.h"
#include <random>
#include <string>

using namespace std;

static std::default_random_engine generator;
static std::uniform_real_distribution<float> distr(0.0, 1.0);

std::string output_vec3(vec3& c);

// the function to return 0-1 results
float randd();

vec3 random_in_unit_sphere();

vec3 random_in_unit_disk();

void get_sphere_uv(const vec3& p, float &u, float&v);