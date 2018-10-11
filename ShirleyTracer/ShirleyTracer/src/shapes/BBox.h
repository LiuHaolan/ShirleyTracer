#pragma once

#include "..\hitable.h"

class BBox {
public:
	BBox():p(0),q(0){}
	BBox(vec3 p_,vec3 q_):p(p_),q(q_){}
	bool hit(const ray& r) const;
	bool inside(vec3 o)const ;

	vec3 p, q;
};