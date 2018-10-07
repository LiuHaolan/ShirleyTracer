#pragma once
#include "..\texture.h"

class Checker3D : public Texture {
public:
//	Checker3D(vec3 c1,vec3 c2,float size_):size(size_), color1(c1),color2(c2){}
	vec3 value(const hit_record& sr) const;
	void set_size(float size_) { size = size_; }
	void set_color1(vec3 color1_) { color1 = color1_; }
	void set_color2(vec3 color2_) { color2 = color2_; }

private:
	float size;
	vec3 color1;
	vec3 color2;
};