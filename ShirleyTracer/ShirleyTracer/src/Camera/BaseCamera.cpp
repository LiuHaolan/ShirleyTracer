#include "BaseCamera.h"

BaseCamera::BaseCamera(void)
	: eye(0, 0, 500),
	lookat(0),
	ra(0),
	up(0, 1, 0),
	u(1, 0, 0),
	v(0, 1, 0),
	w(0, 0, 1),
	exposure_time(1.0)
{}


// ----------------------------------------------------------------- copy constructor

BaseCamera::BaseCamera(const BaseCamera& c)
	: eye(c.eye),
	lookat(c.lookat),
	ra(c.ra),
	up(c.up),
	u(c.u),
	v(c.v),
	w(c.w),
	exposure_time(c.exposure_time)
{}



// ----------------------------------------------------------------- assignment operator

BaseCamera&
BaseCamera::operator= (const BaseCamera& rhs) {
	if (this == &rhs)
		return (*this);

	eye = rhs.eye;
	lookat = rhs.lookat;
	ra = rhs.ra;
	up = rhs.up;
	u = rhs.u;
	v = rhs.v;
	w = rhs.w;
	exposure_time = rhs.exposure_time;

	return (*this);
}


// ----------------------------------------------------------------- destructor

BaseCamera::~BaseCamera(void) {}



//-------------------------------------------------------------- compute_uvw

// This computes an orthornormal basis given the view point, lookat point, and up vector

void
BaseCamera::compute_uvw(void) {
	w = eye - lookat;
	w.make_unit_vector();
	u = cross(up,w);
	u.make_unit_vector();
	v = cross(w,u);

	// take care of the singularity by hardwiring in specific BaseCamera orientations

	if (eye.x() == lookat.x() && eye.z() == lookat.z() && eye.y() > lookat.y()) { // BaseCamera looking vertically down
		u = vec3(0, 0, 1);
		v = vec3(1, 0, 0);
		w = vec3(0, 1, 0);
	}

	if (eye.x() == lookat.x() && eye.z() == lookat.z() && eye.y() < lookat.y()) { // BaseCamera looking vertically up
		u = vec3(1, 0, 0);
		v = vec3(0, 0, 1);
		w = vec3(0, -1, 0);
	}
}
