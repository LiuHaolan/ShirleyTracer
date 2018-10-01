#pragma once

#include "ray.h"
#include "hitable.h"

#define ShadeRec hit_record
//// forward referebce
//class Material;
//class World;
//
//class ShadeRec {
//public:
//	bool				hit_an_object;		// did the ray hit an object?
//	Material* 			material_ptr;		// pointer to the nearest object's material
//	vec3 			hit_point;			// world coordinates of intersection
//	vec3				local_hit_point;	// world coordinates of hit point on untransformed object (used for texture transformations)
//	vec3				normal;				// normal at hit point
//	ray					ray;				// required for specular highlights and area lights
//	int					depth;				// recursion depth
//	vec3			color;				// used in the Chapter 3 only
//	double				t;					// ray parameter
//	float				u;					// texture coordinate
//	float				v;					// texture coordinate
//
//	World& w;
//};
