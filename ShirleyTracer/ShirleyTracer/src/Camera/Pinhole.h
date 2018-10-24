#pragma once

#include "BaseCamera.h"

class Pinhole : public BaseCamera {
public:

	Pinhole();

	Pinhole(const Pinhole& ph);

	virtual BaseCamera*
		clone(void) const;

	Pinhole&
		operator= (const Pinhole& rhs);

	virtual
		~Pinhole();

	void
		set_view_distance(const float vpd);

	void
		set_zoom(const float zoom_factor);

	vec3
		get_direction(const float x, const float y) const;

	vec3 get_eye() {
		return eye;
	}

private:

	float	d;		// view plane distance
	float	zoom;	// zoom factor

};




//-------------------------------------------------------------------------- set_vpd

inline void
Pinhole::set_view_distance(float _d) {
	d = _d;
}



//-------------------------------------------------------------------------- set_zoom

inline void
Pinhole::set_zoom(float zoom_factor) {
	zoom = zoom_factor;
}
