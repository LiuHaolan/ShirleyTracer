
#include "BVH.h"
#include "..\BBox.h"

const static float kEpsilon = 0.0001;

struct {
	bool operator()(hitable* a, hitable* b) const
	{
		BBox box_left = a->get_bounding_box();
		BBox box_right = b->get_bounding_box();
		if (box_left.p.x() <= box_right.p.x())
			return false;
		else
			return true;
	}
} xcom;

struct {
	bool operator()(hitable* a, hitable* b) const
	{
		BBox box_left = a->get_bounding_box();
		BBox box_right = b->get_bounding_box();
		if (box_left.p.y() <= box_right.p.y())
			return false;
		else
			return true;
	}
} ycom;

struct {
	bool operator()(hitable* a, hitable* b) const
	{
		BBox box_left = a->get_bounding_box();
		BBox box_right = b->get_bounding_box();
		if (box_left.p.z() <= box_right.p.z())
			return false;
		else
			return true;
	}
} zcom;

BVH::BVH(std::vector<hitable*>& l,int begin, int end) {
	int axis = (int)(3 * randd());
	if (axis == 0)
		std::sort(l.begin()+begin, l.begin()+end+1, xcom);
	else if (axis == 1)
		std::sort(l.begin()+begin, l.begin()+end+1, ycom);
	else 
		std::sort(l.begin()+begin, l.begin()+end+1, zcom);


	if (end-begin+1 == 1)
		left = right = l[begin];
	else if (end - begin + 1 == 2) {
		left = l[begin];
		right = l[end];
	}
	else {
		left = new BVH(l, begin, begin + (end - begin) / 2);
		right = new BVH(l, begin + (end - begin) / 2, end);
	}

	BBox box_left = left->get_bounding_box();
	BBox box_right = right->get_bounding_box();

	box = surrounding_box(box_left, box_right);
}

BBox BVH::get_bounding_box() const {
	return box;
}

bool BVH::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	if (box.hit(r, t_min, t_max)) {
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);
		if (hit_left && hit_right) {
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;
			return true;
		}
		else if (hit_left) {
			rec = left_rec;
			return true;
		}
		else if (hit_right) {
			rec = right_rec;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool BVH::hitP(const ray& r, float& t) const {
	if (box.hit(r, kEpsilon, MAXFLOAT)) {
		float left_rec, right_rec;
		bool hit_left = left->hitP(r,left_rec);
		bool hit_right = right->hitP(r, right_rec);
		return hit_left || hit_right;
	}
	else
		return false;
}