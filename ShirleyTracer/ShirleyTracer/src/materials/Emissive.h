#pragma once

#include "..\materials.h"

class Emissive : public Material {
private:

	float		ls;		// radiance scaling factor						 
	vec3 	ce;		// color

public:

	// constructors, set functions, etc ...

	void scale_radiance(const float _ls) { ls = _ls; }

	void set_ce(const vec3& c) { ce = c; }
	void set_ce(const float r, const float g, const float b) { ce = vec3(r, g, b); }

	virtual vec3 get_Le(hit_record& sr) const { return ls * ce; }

	virtual vec3 shade(hit_record& sr) {
		lanlog::log_error("arealight using common shading!");
		return vec3(0, 0, 0);
	}

	virtual vec3 area_light_shade(hit_record& sr);

	virtual vec3 path_shade(hit_record& sr);
	virtual vec3 global_path_shade(hit_record& sr);
};
