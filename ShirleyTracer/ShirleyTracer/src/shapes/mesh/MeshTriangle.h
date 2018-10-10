#pragma once

#include "Mesh.h"
#include "..\..\hitable.h"

class MeshTriangle : public hitable {
public:
	Mesh* mesh_ptr;
	int index0, index1, index2;
	vec3 normal;
	float area;

};