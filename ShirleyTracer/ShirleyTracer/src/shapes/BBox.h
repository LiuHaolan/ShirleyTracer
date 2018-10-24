#pragma once

#include "..\hitable.h"



class BBox {
public:
	BBox():p(0),q(0){}
	BBox(vec3 p_,vec3 q_):p(p_),q(q_){}
	bool hit(const ray& r, float tmin, float tmax) const;
	bool inside(vec3 o)const ;

//	static BBox surrounding_box(BBox b1, BBox b2);

	vec3 p, q;
};

BBox surrounding_box(BBox b1, BBox b2);