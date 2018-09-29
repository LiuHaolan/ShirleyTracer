#include "cylinder.h"

bool cylinder::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

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
	else if(yp < y_bottom || yp > y_top){
		return false;
	}
	else {
		float A = xa*xa + za * za;
		float B = (xo*xa + zo * za);
		float C = xo * xo + zo * zo - radius * radius;

		float delta = B*B - A * C;
		float right_t = ((-B - sqrt(delta)) / A);

		if (right_t < t_max && right_t > t_min) {
	//		vec3 hit_point = r.A + right_t * r.B;
			rec.t = right_t;
			rec.p = r.point_at_parameter(rec.t);
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