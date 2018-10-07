#include "constant_color.h"

vec3 ConstantColor::value(const hit_record& sr) const {
	return color;
}