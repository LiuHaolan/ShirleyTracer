#pragma once
#include "..\..\hitable.h"
#include "..\BBox.h"
#include <vector>


class BVH : public hitable {
public:

	BVH(std::vector<hitable*>& l,int begin,int end);
	BBox get_bounding_box() const;
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

private:
	hitable* left;
	hitable* right;

	BBox box;
};