#pragma once


#include "..\Sampler.h"
#include "..\hitable.h"

class Rectangle : public hitable {
public:

	Rectangle(void) : p0(0.0), a(0.0), b(0.0) {}

	Rectangle(const vec3& _p0, const vec3& _a, const vec3& _b): p0(_p0), a(_a),b(_b){
		a_len_squared = (a ).squared_length();
		b_len_squared = (b ).squared_length();
		inv_area = 1.0 / sqrt(a_len_squared*b_len_squared);
	}

	Rectangle(const vec3& _p0, const vec3& _a, const vec3& _b, const vec3& n) : p0(_p0), a(_a), b(_b), normal(n) {
		a_len_squared = (a ).squared_length();
		b_len_squared = (b ).squared_length();
		inv_area = 1.0 / sqrt(a_len_squared*b_len_squared);
	}

	//virtual Rectangle*
	//	clone(void);

	//virtual
	//	~Rectangle(void);

	//Rectangle&
	//	operator= (const Rectangle& rhs);

	//BBox
	//	get_bounding_box(void);

	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
	bool hitP(const ray& r, float& t) const;


	// the following functions are used when the rectangle is a light source

	virtual void set_sampler(Sampler* sampler);

	virtual vec3 sample(void);

	virtual vec3 get_normal(const vec3& p);

	virtual float pdf(const hit_record& sr);

private:

	vec3 		p0;   			// corner vertex 
	vec3		a;				// side
	vec3		b;
	double			a_len_squared;	// square of the length of side a
	double			b_len_squared;	// square of the length of side b
	vec3			normal;

	float			area;			// for rectangular lights
	float			inv_area;		// for rectangular lights
	Sampler*		sampler_ptr;	// for rectangular lights 	

};
