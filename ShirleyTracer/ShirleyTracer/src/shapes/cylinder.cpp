#include "cylinder.h"
#include "Plane.h"
#include "..\utility.h"
static float kEpsilon = 0.0001;

bool cylinder::hitP(const ray& r, float& t) const{
	Plane belowplane(vec3(0, y_bottom, 0), y_norm);
	Plane upplane(vec3(0, y_top, 0), y_norm);

	if (belowplane.hitP(r, t)) {
		vec3 p = r.point_at_parameter(t);

		if ((p - vec3(0, y_bottom, 0)).length() <= radius) {
			if (dot(r.B, vec3(0, -1, 0)) < 0) {
				return true;
			}
		}
	}

	if (upplane.hitP(r, t)) {
		vec3 p = r.point_at_parameter(t);

		if ((p - vec3(0, y_top, 0)).length() <= radius) {
			if (dot(r.B, vec3(0, 1, 0)) < 0) {
				return true;
			}
		}
	}

	float xa = r.B.x();
	float za = r.B.z();
	float xo = r.A.x();
	float zo = r.A.z();

	float yo = r.A.y();
	float ya = r.B.y();

	t = (-1)*(xo*xa + zo * za) / (xa*xa + za * za);
	float xp = xo + t * xa;
	float yp = yo + t * ya;
	float zp = zo + t * za;

	float r_square = xp * xp + zp * zp;
	if (r_square > radius * radius) {
		return false;
	}
	/*else if(yp < y_bottom || yp > y_top){
		return false;
	}*/
	else {
		float A = xa * xa + za * za;
		float B = (xo*xa + zo * za);
		float C = xo * xo + zo * zo - radius * radius;

		float delta = B * B - A * C;

		// you need to know if A is negative
		// but here is no, cause A equals two sum of quadra
		float right_t = ((-B - sqrt(delta)) / A);

		if (right_t < MAXFLOAT && right_t > kEpsilon) {
			//		vec3 hit_point = r.A + right_t * r.B;
			t = right_t;
			vec3 p = r.point_at_parameter(t);

			if (p.y() < y_bottom || p.y() > y_top) {
				return false;
			}
			

			// texture mapping unimplemented
		
			return true;
		}
		else {
			return false;
		}

	}

	return false;
}

bool cylinder::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	Plane belowplane(vec3(0, y_bottom, 0), y_norm);
	Plane upplane(vec3(0, y_top, 0), y_norm);

	if (belowplane.hit(r, t_min, t_max, rec)) {
		if ((rec.p - vec3(0, y_bottom, 0)).length() <= radius) {
			if (dot(r.B, vec3(0, -1, 0)) < 0) {
				rec.mat_ptr = mat;
				rec.normal = vec3(0, -1, 0);
				return true;
			}
		}
	}

	if (upplane.hit(r, t_min, t_max, rec)) {
		if ((rec.p - vec3(0, y_top, 0)).length() <= radius) {
			if (dot(r.B, vec3(0, 1, 0)) < 0) {
				rec.mat_ptr = mat;
				rec.normal = vec3(0, 1, 0);
				return true;
			}
		}
	}

	float xa = r.B.x();
	float za = r.B.z();
	float xo = r.A.x();
	float zo = r.A.z();
	
	float yo = r.A.y();
	float ya = r.B.y();

	float t = (-1)*(xo*xa + zo * za) / (xa*xa + za * za);
	float xp = xo + t * xa;
	float yp = yo + t * ya;
	float zp = zo + t * za;

	float r_square = xp * xp + zp * zp;
	if (r_square > radius * radius) {
		return false;
	}
	/*else if(yp < y_bottom || yp > y_top){
		return false;
	}*/
	else {
		float A = xa*xa + za * za;
		float B = (xo*xa + zo * za);
		float C = xo * xo + zo * zo - radius * radius;

		float delta = B*B - A * C;

		// you need to know if A is negative
		// but here is no, cause A equals two sum of quadra
		float right_t = ((-B - sqrt(delta)) / A);

		if (right_t < t_max && right_t > t_min) {
	//		vec3 hit_point = r.A + right_t * r.B;
			rec.t = right_t;
			rec.p = r.point_at_parameter(rec.t);

			if (rec.p.y() < y_bottom || rec.p.y() > y_top) {
				return false;
			}
			rec.normal = vec3(rec.p.x(),0,rec.p.z());
			rec.mat_ptr = mat;

			// texture mapping unimplemented
			rec.u = 0;
			rec.v = 0;
			return true;
		}
		else {
			return false;
		}
		
	}

	return false;
}