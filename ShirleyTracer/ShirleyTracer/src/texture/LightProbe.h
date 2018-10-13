#pragma once
#include "mapping.h"
typedef enum {
	light_prob,
	panoramic
} MapType;

class LightProbe : public Mapping {
public:
	void
		get_texel_coordinates(const 	vec3& 	hit_point,
			const 	int 		xres,
			const 	int 		yres,
			int& 		row,
			int& 		column) const;
	void set_map_type(MapType t) {
		maptype = t;
	}
private:
	MapType maptype;
};