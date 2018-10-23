#pragma once


#include "BBox.h"
#include "mesh\MeshTriangle.h"
#include "Compound.h"

class MeshAggregate : public Compound{
public:
	MeshAggregate() {  }
	MeshAggregate(Mesh* ptr);
	//virtual BBox get_bounding_box();
	//void setup_cells();
	//bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	//bool hitP(const ray& r, float& t) const;

private:
	//vector<hitable*> cells;
	//BBox bbox;
	//int nx, ny, nz; //number of cells in the three directions;
	//vec3 min_coordinates();
	//vec3 max_coordinates();

};
