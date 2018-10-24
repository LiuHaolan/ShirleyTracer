#include "Pinhole.h"

Pinhole::Pinhole(void)
	: BaseCamera(),
	d(500),
	zoom(1.0)
{}


// ----------------------------------------------------------------------------- copy constructor

Pinhole::Pinhole(const Pinhole& c)
	: BaseCamera(c),
	d(c.d),
	zoom(c.zoom)
{}


// ----------------------------------------------------------------------------- clone

BaseCamera*
Pinhole::clone(void) const {
	return (new Pinhole(*this));
}



// ----------------------------------------------------------------------------- assignment operator

Pinhole&
Pinhole::operator= (const Pinhole& rhs) {
	if (this == &rhs)
		return (*this);

	BaseCamera::operator= (rhs);

	d = rhs.d;
	zoom = rhs.zoom;

	return (*this);
}


// ----------------------------------------------------------------------------- destructor

Pinhole::~Pinhole(void) {}


// ----------------------------------------------------------------------------- get_direction

vec3 Pinhole::get_direction(const float x,const float y) const {
	vec3 dir = x * u + y * v - d * w;
	dir.make_unit_vector();

	return(dir);
}



