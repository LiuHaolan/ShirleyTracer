#include "instance.h"

bool Instance::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	ray inv_ray(r);
	inv_ray.A = inv_matrix * r.A;
	inv_ray.B = inv_matrix * r.B;

	if (object_ptr->hit(inv_ray, t_min, t_max, rec)) {
		rec.normal = inv_matrix.normalmulti(rec.normal);
		rec.normal.make_unit_vector();

		if (object_ptr->get_material() == 0)
			rec.mat_ptr = mat;

		if (!transform_the_texture)
			rec.local_hit_point = r.point_at_parameter(rec.t);

	//	rec.p = r.A + rec.t*r.B;

		return true;
	}

	return false;
}


void Instance::translate(const vec3& trans) {
	Matrix inv_translation_matrix;				// temporary inverse translation matrix	
	inv_translation_matrix.set_identity();

	inv_translation_matrix.m[0][3] = -trans.x();
	inv_translation_matrix.m[1][3] = -trans.y();
	inv_translation_matrix.m[2][3] = -trans.z();

	inv_matrix = inv_matrix * inv_translation_matrix;
}

void Instance::translate(const double dx, const double dy, const double dz) {
	Matrix inv_translation_matrix;				// temporary inverse translation matrix	
	inv_translation_matrix.set_identity();

	inv_translation_matrix.m[0][3] = -dx;
	inv_translation_matrix.m[1][3] = -dy;
	inv_translation_matrix.m[2][3] = -dz;

	inv_matrix = inv_matrix * inv_translation_matrix;
}

void Instance::scale(const vec3& s) {
	Matrix	inv_scaling_matrix;			// temporary inverse scaling matrix
	inv_scaling_matrix.set_identity();

	inv_scaling_matrix.m[0][0] = 1.0 / s.x();
	inv_scaling_matrix.m[1][1] = 1.0 / s.y();
	inv_scaling_matrix.m[2][2] = 1.0 / s.z();

	inv_matrix = inv_matrix * inv_scaling_matrix;
}

void Instance::scale(const double a, const double b, const double c) {
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