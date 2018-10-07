#include "Matrix.h"

Matrix::Matrix() {
	for (int j = 0; j < 16; j++)
		m[j / 4][j % 4] = 0;
}

Matrix& 										// assignment operator
Matrix::operator= (const Matrix& rhs) {
	for (int j = 0; j < 16; j++)
		m[j / 4][j % 4] = rhs.m[j/4][j%4];

	return *this;
}


// you can optimize here!
Matrix 											// multiplication of two matrices
Matrix::operator* (const Matrix& mat) const {
	Matrix* res = new Matrix;
	for(int j=0;j<4;j++)
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				res->m[j][i] += this->m[j][k] * mat.m[k][i];
			}
		}

	return *res;
}

Matrix 											// divsion by a double
Matrix::operator/ (const double d) {
	for (int j = 0; j < 16; j++)
		m[j / 4][j % 4] /= d;

	return *this;
}

void Matrix::set_identity(void) {
	for (int j = 0; j < 4; j++)
		m[j][j] = 1.0;
}


vec3 Matrix::operator*(const vec3& rhs) const {
	return vec3(m[0][0] * rhs.x() + m[0][1] * rhs.y() + m[0][2] * rhs.z(),  \
		m[1][0] * rhs.x() + m[1][1] * rhs.y() + m[1][2] * rhs.z(), \
		m[2][0] * rhs.x() + m[2][1] * rhs.y() + m[2][2] * rhs.z());
}

vec3 Matrix::normalmulti(const vec3& rhs) const {
	return vec3(m[0][0] * rhs.x() + m[1][0] * rhs.y() + m[2][0] * rhs.z(), \
		m[0][1] * rhs.x() + m[1][1] * rhs.y() + m[2][1] * rhs.z(), \
		m[0][2] * rhs.x() + m[1][2] * rhs.y() + m[2][2] * rhs.z());
}