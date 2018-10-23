#pragma once

#include "..\hitable.h"
#include "..\Matrix.h"
#include "..\utility.h"

class Instance : public hitable {
public:
//	Instance();
	Instance(hitable* ptr) : object_ptr(ptr) { inv_matrix.set_identity(); forward_matrix.set_identity(); transform_the_texture = false; }
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

	void translate(const vec3& trans);
	void translate(const double dx, const double dy, const double dz);

	void scale(const vec3& s);
	void scale(const double a, const double b, const double c);
	void transform_texture(bool transform) {
		transform_the_texture = transform;
	}

	virtual void rotate_x(const double r);
	virtual void rotate_y(const double r);
	virtual void rotate_z(const double r);
	void rotate_axis(const double theta, const vec3& axis);

	bool InvertMatrix(const double m[16], double invOut[16]);

	Matrix get_forward() {
		return forward_matrix;
	}
	vec3 transform(const vec3& o) const;
private:
	hitable* object_ptr;
	Matrix inv_matrix;
	Matrix forward_matrix;
	bool transform_the_texture;
};

