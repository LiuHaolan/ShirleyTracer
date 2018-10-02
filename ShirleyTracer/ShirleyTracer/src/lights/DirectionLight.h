#pragma once
#include "..\light.h"

class DirectionLight : public Light {
public:
	DirectionLight():ls(1.0),color(vec3(1.0,1.0,1.0)),dir(vec3(0.0,0.0,0.0)){}

	void
		scale_radiance(const float b);

	void
		set_color(const float c);

	void
		set_color(const vec3& c);

	void
		set_color(const float r, const float g, const float b);

	void
		set_direction(vec3 d);

	vec3 get_direction(hit_record& sr) const;
	vec3 L(ShadeRec& s) const;

private:

	float		ls;
	vec3	color;
	vec3	dir;		// direction the light comes from
};
