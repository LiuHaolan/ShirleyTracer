#pragma once

#include "..\vec3.h"
//#include "vec3.h"

class World;  // can't #include "World" here because World contains a BaseCamera pointer

//--------------------------------------------------------------------- class BaseCamera

class BaseCamera {
public:

	BaseCamera();   							// default constructor

	BaseCamera(const BaseCamera& BaseCamera);			// copy constructor

	virtual BaseCamera*							// virtual copy constructor
		clone(void) const = 0;

	virtual
		~BaseCamera();

	void
		set_eye(const vec3& p);

	void
		set_eye(const float x, const float y, const float z);

	void
		set_lookat(const vec3& p);

	void
		set_lookat(const float x, const float y, const float z);

	void
		set_up_vector(const vec3& u);

	void
		set_up_vector(const float x, const float y, const float z);

	void
		set_roll(const float ra);

	void
		set_exposure_time(const float exposure);

	void
		compute_uvw(void);


protected:

	vec3			eye;				// eye point
	vec3			lookat; 			// lookat point
	float			ra;					// roll angle
	vec3		u, v, w;			// orthonormal basis vectors
	vec3		up;					// up vector
	float			exposure_time;

	BaseCamera& 							// assignment operator
		operator= (const BaseCamera& BaseCamera);
};


// inlined access functions


// ----------------------------------------------------------------- set_eye

inline void
BaseCamera::set_eye(const vec3& p) {
	eye = p;
}


// ----------------------------------------------------------------- set_eye

inline void
BaseCamera::set_eye(const float x, const float y, const float z) {
	eye[0] = x; eye[1] = y; eye[2] = z;
}


// ----------------------------------------------------------------- set_lookat

inline void
BaseCamera::set_lookat(const vec3& p) {
	lookat = p;
}


// ----------------------------------------------------------------- set_lookat

inline void
BaseCamera::set_lookat(const float x, const float y, const float z) {
	lookat[0] = x; lookat[1] = y; lookat[2] = z;
}


// ----------------------------------------------------------------- set_up_vector

inline void
BaseCamera::set_up_vector(const vec3& u) {
	up = u;
}


// ----------------------------------------------------------------- set_up_vector

inline void
BaseCamera::set_up_vector(const float x, const float y, const float z) {
	up[0] = x; up[1] = y; up[2] = z;
}


// ----------------------------------------------------------------- set_roll

inline void
BaseCamera::set_roll(const float r) {
	ra = r;
}


// ----------------------------------------------------------------- set_exposure_time

inline void
BaseCamera::set_exposure_time(const float exposure) {
	exposure_time = exposure;
}
