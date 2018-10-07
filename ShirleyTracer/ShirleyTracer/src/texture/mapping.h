#pragma once


#include <math.h>

#include "..\vec3.h"

class Mapping {
public:

	virtual void
		get_texel_coordinates(const 	vec3& 	hit_point,
			const 	int 		xres,
			const 	int 		yres,
			int& 		row,
			int& 		column) const = 0;
};
