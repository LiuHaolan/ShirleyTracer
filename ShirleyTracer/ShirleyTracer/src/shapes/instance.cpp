#include "instance.h"

bool Instance::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	ray inv_ray(r);
	inv_ray.A = inv_matrix * r.A;
	inv_ray.B = inv_matrix.dirmulti(r.B);

	if (object_ptr->hit(inv_ray, t_min, t_max, rec)) {
		rec.normal = inv_matrix.normalmulti(rec.normal);
		rec.normal.make_unit_vector();

		if (object_ptr->get_material() == 0)
			rec.mat_ptr = mat;

		if (!transform_the_texture)
			rec.local_hit_point = r.point_at_parameter(rec.t);

		rec.p = r.A + rec.t*r.B;

		return true;
	}

	return false;
}


void Instance::translate(const vec3& trans) {
	translate(trans[0], trans[1], trans[2]);
}

void Instance::translate(const double dx, const double dy, const double dz) {
	
	Matrix translation_matrix;				// temporary inverse translation matrix	
	translation_matrix.set_identity();

	translation_matrix.m[0][3] = dx;
	translation_matrix.m[1][3] = dy;
	translation_matrix.m[2][3] = dz;

	forward_matrix =  translation_matrix* forward_matrix ;

	Matrix inv_translation_matrix;				// temporary inverse translation matrix	
	inv_translation_matrix.set_identity();

	inv_translation_matrix.m[0][3] = -dx;
	inv_translation_matrix.m[1][3] = -dy;
	inv_translation_matrix.m[2][3] = -dz;

	inv_matrix = inv_matrix * inv_translation_matrix;
}

void Instance::scale(const vec3& s) {
	scale(s[0], s[1], s[2]);
}

void Instance::scale(const double a, const double b, const double c) {
	Matrix	scaling_matrix;			// temporary inverse scaling matrix
	scaling_matrix.set_identity();

	scaling_matrix.m[0][0] = a;
	scaling_matrix.m[1][1] = b;
	scaling_matrix.m[2][2] = c;

	forward_matrix = scaling_matrix*forward_matrix;
	
	Matrix	inv_scaling_matrix;			// temporary inverse scaling matrix
	inv_scaling_matrix.set_identity();

	inv_scaling_matrix.m[0][0] = 1.0 / a;
	inv_scaling_matrix.m[1][1] = 1.0 / b;
	inv_scaling_matrix.m[2][2] = 1.0 / c;

	inv_matrix = inv_matrix * inv_scaling_matrix;
}

void Instance::rotate_x(const double theta) {

	double sin_theta = sin(theta * M_PI / 180.0);
	double cos_theta = cos(theta * M_PI /180.0);

	Matrix inv_x_rotation_matrix;					// temporary inverse rotation matrix about x axis
	inv_x_rotation_matrix.set_identity();

	inv_x_rotation_matrix.m[1][1] = cos_theta;
	inv_x_rotation_matrix.m[1][2] = sin_theta;
	inv_x_rotation_matrix.m[2][1] = -sin_theta;
	inv_x_rotation_matrix.m[2][2] = cos_theta;

	inv_matrix = inv_matrix * inv_x_rotation_matrix;
}

void Instance::rotate_y(const double theta) {
	double sin_theta = sin(theta * M_PI / 180.0);
	double cos_theta = cos(theta * M_PI / 180.0);

	Matrix inv_y_rotation_matrix;					// temporary inverse rotation matrix about y axis
	inv_y_rotation_matrix.set_identity();

	inv_y_rotation_matrix.m[0][0] = cos_theta;
	inv_y_rotation_matrix.m[0][2] = -sin_theta;
	inv_y_rotation_matrix.m[2][0] = sin_theta;
	inv_y_rotation_matrix.m[2][2] = cos_theta;

	inv_matrix = inv_matrix * inv_y_rotation_matrix;
}

void Instance::rotate_z(const double theta) {
	double sin_theta = sin(theta * M_PI / 180.0);
	double cos_theta = cos(theta * M_PI / 180.0);

	Matrix inv_z_rotation_matrix;					// temporary inverse rotation matrix about y axis	
	inv_z_rotation_matrix.set_identity();

	inv_z_rotation_matrix.m[0][0] = cos_theta;
	inv_z_rotation_matrix.m[0][1] = sin_theta;
	inv_z_rotation_matrix.m[1][0] = -sin_theta;
	inv_z_rotation_matrix.m[1][1] = cos_theta;

	inv_matrix = inv_matrix * inv_z_rotation_matrix;
}

void Instance::rotate_axis(const double theta, const vec3& axis) {
	double sin_theta = sin(theta * M_PI / 180.0);
	double cos_theta = cos(theta * M_PI / 180.0);
	vec3 a(axis);
	a.make_unit_vector();

	Matrix axis_rotation_matrix; // temporary rotation matrix about z axis
	axis_rotation_matrix.set_identity();

	// Compute rotation of first basis vector
	axis_rotation_matrix.m[0][0] = a.x() * a.x() + (1 - a.x() * a.x()) * cos_theta;
	axis_rotation_matrix.m[0][1] = a.x() * a.y() * (1 - cos_theta) - a.z() * sin_theta;
	axis_rotation_matrix.m[0][2] = a.x() * a.z() * (1 - cos_theta) + a.y() * sin_theta;
	axis_rotation_matrix.m[0][3] = 0;

	// Compute rotations of second and third basis vectors
	axis_rotation_matrix.m[1][0] = a.x() * a.y() * (1 - cos_theta) + a.z() * sin_theta;
	axis_rotation_matrix.m[1][1] = a.y() * a.y() + (1 - a.y() * a.y()) * cos_theta;
	axis_rotation_matrix.m[1][2] = a.y() * a.z() * (1 - cos_theta) - a.x() * sin_theta;
	axis_rotation_matrix.m[1][3] = 0;

	axis_rotation_matrix.m[2][0] = a.x() * a.z() * (1 - cos_theta) - a.y() * sin_theta;
	axis_rotation_matrix.m[2][1] = a.y() * a.z()* (1 - cos_theta) + a.x() * sin_theta;
	axis_rotation_matrix.m[2][2] = a.z()* a.z() + (1 - a.z() * a.z()) * cos_theta;
	axis_rotation_matrix.m[2][3] = 0;

	forward_matrix = axis_rotation_matrix * forward_matrix;


	Matrix inv_axis_rotation_matrix; // temporary inverse rotation matrix about axis
	inv_axis_rotation_matrix.set_identity();
	sin_theta = sin_theta * (-1);     // rotate in the opposite direction.
	cos_theta = cos_theta * (1);

	// Compute rotation of first basis vector
	inv_axis_rotation_matrix.m[0][0] = a.x() * a.x() + (1 - a.x() * a.x()) * cos_theta;
	inv_axis_rotation_matrix.m[0][1] = a.x() * a.y()* (1 - cos_theta) - a.z() * sin_theta;
	inv_axis_rotation_matrix.m[0][2] = a.x() * a.z() * (1 - cos_theta) + a.y() * sin_theta;
	inv_axis_rotation_matrix.m[0][3] = 0;

	// Compute rotations of second and third basis vectors
	inv_axis_rotation_matrix.m[1][0] = a.x()* a.y() * (1 - cos_theta) + a.z() * sin_theta;
	inv_axis_rotation_matrix.m[1][1] = a.y() * a.y() + (1 - a.y() * a.y()) * cos_theta;
	inv_axis_rotation_matrix.m[1][2] = a.y()* a.z() * (1 - cos_theta) - a.x() * sin_theta;
	inv_axis_rotation_matrix.m[1][3] = 0;

	inv_axis_rotation_matrix.m[2][0] = a.x() * a.z() * (1 - cos_theta) - a.y()* sin_theta;
	inv_axis_rotation_matrix.m[2][1] = a.y() * a.z() * (1 - cos_theta) + a.x() * sin_theta;
	inv_axis_rotation_matrix.m[2][2] = a.z() * a.z() + (1 - a.z() * a.z()) * cos_theta;
	inv_axis_rotation_matrix.m[2][3] = 0;

	inv_matrix = inv_matrix * inv_axis_rotation_matrix;
}

vec3 Instance::transform(const vec3& o) const {
	return forward_matrix*o;
}

bool Instance::InvertMatrix(const double m[16], double invOut[16])
{
	double inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}