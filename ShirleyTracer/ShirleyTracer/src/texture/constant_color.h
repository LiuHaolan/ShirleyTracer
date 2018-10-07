#pragma once

#include "..\texture.h"

class ConstantColor : public Texture {
public:
	ConstantColor(const vec3& c): color(c){}
	void set_color(const vec3& c) { color = c; }
	vec3 value(const hit_record& sr) const;

private:
	vec3 color;
};