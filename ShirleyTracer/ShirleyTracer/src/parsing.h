#pragma once
#include <stdio.h>
#include "vec3.h"
#include "World.h"
#include <vector>
#include <string>


#include "lights/PointLight.h"
#include "shapes/sphere.h"
#include "shapes/Instance.h"
#include "shapes/mesh/MeshTriangle.h"
#include "shapes/accelerator/Grid.h"
#include "texture/ImageTexture.h"
#include "texture/constant_color.h"
#include "materials/SVPhong.h"


#include "BART/animation.h"

using namespace std;

// a global record to collect some information

static vector<string> dynamic_lights_name;
static vector<int> dynamic_lights_ind;



typedef float Vec2f[2];
typedef float Vec3f[3];
typedef float Vec4f[4];
#define X 0 
#define Y 1  
#define Z 2  
#define W 3  

#define R 0  
#define G 1  
#define B 2  
#define A 3 /* alpha */

struct viewParams {
	vec3 from;
	vec3 at;
	vec3 up;
	float fov_angle;		// you read some degree.
	float hither;
	int resx;
	int resy;
};

struct transform_global {
	std::vector<vec3> translate;
	std::vector<vec3> rotate_axis;
	std::vector<float> degree;
	std::vector<vec3> scale;
};

extern struct transform_global* transform_st;
extern struct viewParams* mViewParams;
extern struct AnimationParams* mAnimationParams;
extern struct AnimationList* mAnimations;
extern float g_amb[3];
extern float g_diff[3];
extern float g_spec[3];
extern float g_Shine;
extern float g_T;
extern float g_index_of_refraction;

extern 

bool viParseFile(FILE* f, World* ptr);