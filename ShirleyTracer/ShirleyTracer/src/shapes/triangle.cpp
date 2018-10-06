#include "triangle.h"
#include "..\utility.h"
static float kEpsilon = 0.0001;

bool Triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	const vec3& p1 = vertex[0];
	const vec3& p2 = vertex[1];
	const vec3& p3 = vertex[2];

	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;

	//caculating the normal vector
	vec3 normal = cross(e1,e2);

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

	if (dot(normal, r.B) < 0)
		rec.normal = unit_vector(normal);			// something might be wrong here!
	else
		rec.normal = -unit_vector(normal);
	rec.mat_ptr = mat;

	return true;

}

bool Triangle::hitP(const ray& r, float& t) const {
	const vec3& p1 = vertex[0];
	const vec3& p2 = vertex[1];
	const vec3& p3 = vertex[2];

	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;

	//caculating the normal vector
	vec3 normal = cross(e1, e2);

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

	t = dot(e2, s2)*invDivisor;
	if (t<kEpsilon || t>MAXFLOAT)
		return false;

	vec3 pos = r.A + t * r.B;
//	float dist = (pos - r.A).length();

	return true;
}