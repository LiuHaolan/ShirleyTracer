#pragma once
#include "vec3.h"

class Matrix {
public:
	double m[4][4];
	Matrix();

	Matrix& 										// assignment operator
		operator= (const Matrix& rhs);

	Matrix 											// multiplication of two matrices
		operator* (const Matrix& mat) const;

	Matrix 											// divsion by a double
		operator/ (const double d);

	vec3 operator*(const vec3& rhs) const;

	vec3 normalmulti(const vec3& rhs) const;

	void											// set to the identity matrix
		set_identity(void);

};