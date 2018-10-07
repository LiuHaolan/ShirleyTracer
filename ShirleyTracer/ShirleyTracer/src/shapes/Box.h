#pragma once

#include "..\hitable.h"

class Box : public hitable {
public:
	Box(vec3 p_,vec3 q_):p(p_),q(q_){}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;

private:
	vec3 p, q;

};
