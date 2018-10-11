#pragma once

#include "Mesh.h"
#include "..\..\hitable.h"

class MeshTriangle : public hitable {
public:
	MeshTriangle(Mesh* ptr,int t0,int t1,int t2);

	BBox get_bounding_box();
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

	Mesh* mesh_ptr;
	int index0, index1, index2;
//	vec3 normal;
	//float area;

};