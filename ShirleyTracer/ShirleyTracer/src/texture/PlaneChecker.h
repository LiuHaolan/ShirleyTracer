#pragma once
#include "..\texture.h"

class PlaneChecker : public Texture {
public:
	vec3 value(const hit_record& sr) const;
	void set_size(float size_) { size = size_; }
	void set_color1(vec3 color1_) { color1 = color1_; }
	void set_color2(vec3 color2_) { color2 = color2_; }
	void set_outline_width(float c) { outline_width = c; }
	void set_outline_color(vec3 c) { outline_color = c; }
private:
	vec3 outline_color;
	float outline_width;
	float size;
	vec3 color1;
	vec3 color2;
};