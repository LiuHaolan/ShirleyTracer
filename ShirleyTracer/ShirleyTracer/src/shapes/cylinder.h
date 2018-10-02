#pragma once

#include "../hitable.h"

// compute the distance between two tays


class cylinder : public hitable {
public:
	cylinder(double bottom_,double top_,double r_):y_bottom(bottom_), y_top(top_), radius(r_), y_norm(vec3(0,1,0)){}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;


private:
	double y_bottom;
	double y_top;
	double radius;
	vec3 y_norm;

};

