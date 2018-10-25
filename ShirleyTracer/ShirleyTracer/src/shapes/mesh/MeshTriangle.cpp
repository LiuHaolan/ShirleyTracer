
#include <cmath>

#include "MeshTriangle.h"
#include "..\BBox.h"



const static float kEpsilon = 0.0001;

inline float qz(float k) {
	return (k - floor(k));
}

MeshTriangle::MeshTriangle(Mesh* ptr,int t0, int t1, int t2,int f) {
	mesh_ptr = ptr;
	index0 = t0;
	index1 = t1;
	index2 = t2;
	
	// for uniform material
	mat = ptr->material;

	face_ind = f;
}

BBox MeshTriangle::get_bounding_box() const{
	vec3 a = mesh_ptr->vertices[index0];
	vec3 b = mesh_ptr->vertices[index1];
	vec3 c = mesh_ptr->vertices[index2];

	float x0 = min(min(a.x(), b.x()), c.x()) - 0.0001;
	float y0 = min(min(a.y(), b.y()), c.y()) - 0.0001;
	float z0 = min(min(a.z(), b.z()), c.z()) - 0.0001;
	float x1 = max(max(a.x(), b.x()), c.x()) + 0.0001;
	float y1 = max(max(a.y(), b.y()), c.y()) + 0.0001;
	float z1 = max(max(a.z(), b.z()), c.z()) + 0.0001;


	return BBox(vec3(x0,y0,z0), vec3(x1,y1,z1));
}

bool MeshTriangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	BBox box = get_bounding_box();
	if (!box.hit(r, t_min, t_max))
		return false;

	vec3 p1(mesh_ptr->vertices[index0]);
	vec3 p2(mesh_ptr->vertices[index1]);
	vec3 p3(mesh_ptr->vertices[index2]);
	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;

	vec3 s1 = cross(r.B, e2);
	float divisor = dot(s1, e1);

	if (divisor == 0.)
		return false;
	float invDivisor = 1.f / divisor;

	vec3 s = r.A - p1;
	float b1 = dot(s, s1)*invDivisor;
	if (b1<0. || b1>1.)
		return false;

	vec3 s2 = cross(s, e1);
	float b2 = dot(r.B, s2)*invDivisor;
	if (b2 < 0. || b1 + b2 >1.)
		return false;

	float t = dot(e2, s2)*invDivisor;
	if (t<t_min || t>t_max)
		return false;

	vec3 pos = r.A + t * r.B;
	//	float dist = (pos - r.A).length();

	rec.p = pos;
	rec.t = t;

	if (mesh_ptr->u.size()) {
	//	//calculating the uv
	//	float w1 = cross(pos - p2, pos - p3).length();
	//	float w2 = cross(pos - p1, pos - p3).length();
	//	float w3 = cross(pos - p1, pos - p2).length();
	//	float sum = w1 + w2 + w3;
	//	w1 = w1 / sum;
	//	w2 = w2 / sum;
	//	w3 = w3 / sum;

	////	float uscale = max(max(fabs(mesh_ptr->u[index1]), fabs(mesh_ptr->u[index2])), fabs(mesh_ptr->u[index0]));
	////	float vscale = max(max(fabs(mesh_ptr->v[index1]), fabs(mesh_ptr->v[index2])), fabs(mesh_ptr->v[index0]));
	//	rec.u = mesh_ptr->u[index0] * w1 + mesh_ptr->u[index1] * w2 + mesh_ptr->u[index2] * w3;
	//	rec.v = mesh_ptr->v[index0] * w1 + mesh_ptr->v[index1] * w2 + mesh_ptr->v[index2] * w3;

		double a = p1.x() - p2.x(), b = p1.x() - p3.x(), c = r.B.x(), d = p1.x() - r.A.x();
		double e = p1.y() - p2.y(), f = p1.y() - p3.y(), g = r.B.y(), h = p1.y() - r.A.y();
		double i = p1.z() - p2.z(), j = p1.z() - p3.z(), k = r.B.z(), l = p1.z() - r.A.z();

		double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
		double q = g * i - e * k, s = e * j - f * i;

		double inv_denom = 1.0 / (a * m + b * q + c * s);

		double e1 = d * m - b * n - c * p;
		double beta = e1 * inv_denom;

		if (beta < 0.0)
			return (false);

		double r = e * l - h * i;
		double e2 = a * n + d * q + c * r;
		double gamma = e2 * inv_denom;

		rec.u = interpolate_u(beta, gamma);
		rec.v = interpolate_v(beta, gamma);
	}

	//caculating the normal vector
	vec3 normal = cross(e1, e2);
	//flat shading

	// if(smooth)
	if (mesh_ptr->has_normals) {
		
		int i1 = mesh_ptr->facenormal_indices[face_ind][0];
		int i2 = mesh_ptr->facenormal_indices[face_ind][1];
		int i3 = mesh_ptr->facenormal_indices[face_ind][2];

		// weird! some index get out of range
		assert(i1 < mesh_ptr->normals.size());
		assert(i2 < mesh_ptr->normals.size());
		assert(i3 < mesh_ptr->normals.size());
			

		vec3 n1 = mesh_ptr->normals[i1];
		vec3 n2 = mesh_ptr->normals[i2];
		vec3 n3 = mesh_ptr->normals[i3];

		n1.make_unit_vector();
		n2.make_unit_vector();
		n2.make_unit_vector();
		float w1 = cross(pos - p2, pos - p3).length();
		float w2 = cross(pos - p1, pos - p3).length();
		float w3 = cross(pos - p1, pos - p2).length();
		float w = w1 + w2 + w3;
		normal = (w1*n1 + w2 * n2 + w3 * n3) / w;
	}

//	assert(!isnan(r.B.x()));
	if (dot(normal, r.B) < 0)
		rec.normal = unit_vector(normal);			// something might be wrong here!
	else
		rec.normal = -unit_vector(normal);
	rec.mat_ptr = mat;

	//problem here
	rec.topo_normal = rec.normal;

	return true;
}

bool MeshTriangle::hitP(const ray& r, float& t) const {
	vec3 v0(mesh_ptr->vertices[index0]);
	vec3 v1(mesh_ptr->vertices[index1]);
	vec3 v2(mesh_ptr->vertices[index2]);

	double a = v0.x() - v1.x(), b = v0.x() - v2.x(), c = r.A.x(), d = v0.x() - r.B.x();
	double e = v0.y() - v1.y(), f = v0.y() - v2.y(), g = r.A.y(), h = v0.y() - r.B.y();
	double i = v0.z() - v1.z(), j = v0.z() - v2.z(), k = r.A.z(), l = v0.z() - r.B.z();

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i, rr = e * l - h * i;

	double inv_denom = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double e2 = a * n + d * q + c * rr;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * rr + d * s;
	t = e3 * inv_denom;

	if (t < kEpsilon )
		return (false);

	return (true);
}


float
MeshTriangle::interpolate_u(const float beta, const float gamma) const {
	return((1 - beta - gamma) * mesh_ptr->u[mesh_ptr->faceuv_indices[face_ind][0]]
		+ beta * mesh_ptr->u[mesh_ptr->faceuv_indices[face_ind][1]]
		+ gamma * mesh_ptr->u[mesh_ptr->faceuv_indices[face_ind][2]]);
}


// ---------------------------------------------------------------- interpolate_v
// this is used for texture mapping in Chapter 29

float
MeshTriangle::interpolate_v(const float beta, const float gamma) const {
	return((1 - beta - gamma) * mesh_ptr->v[index0]
		+ beta * mesh_ptr->v[index1]
		+ gamma * mesh_ptr->v[index2]);
}
