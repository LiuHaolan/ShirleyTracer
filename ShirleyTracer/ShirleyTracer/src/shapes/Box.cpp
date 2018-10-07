#include "Box.h"
#include <algorithm>

bool Box::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	// this is the same as Listing 19.1 down to the statement float t0, t1;

	double x0 = min(p.x(), q.x());
	double x1 = max(p.x(), q.x());
	double y0 = min(p.y(), q.y());
	double y1 = max(p.y(), q.y());
	double z0 = min(p.z(), q.z());
	double z1 = max(p.z(), q.z());

	double ox = r.A.x(); 
	double oy = r.A.y();
	double oz = r.A.z();  
	double dx = r.B.x();
	double dy = r.B.y();
	double dz = r.B.z();

	double tx1 = (x0 - ox) / dx;
	double tx2 = (x1 - ox) / dx;
	double ty1 = (y0 - oy) / dy;
	double ty2 = (y1 - oy) / dy;
	double tz1 = (z0 - oz) / dz;
	double tz2 = (z1 - oz) / dz;

	double tmin = max(max(min(tx1, tx2), min(ty1, ty2)),min(tz1, tz2));
	double tmax = min(min(max(tx1, tx2), max(ty1, ty2)), max(tz1, tz2));

	if (tmax < tmin) {
		return false;
	}
	else {
		if (tmin < t_min || tmin >t_max)
			return false;
		else {
			rec.t = tmin;
			rec.p = r.point_at_parameter(tmin);
			rec.mat_ptr = mat;

			// local hit_point
			rec.local_hit_point = rec.p;

			if (abs(rec.p.x() - x0)<0.001)
				rec.normal = vec3(-1, 0, 0);
			else if (abs(rec.p.x() - x1) < 0.001)
				rec.normal = vec3(1, 0, 0);
			else if (abs(rec.p.y() - y0) < 0.001)
				rec.normal = vec3(0, -1, 0);
			else if (abs(rec.p.y() - y1) < 0.001)
				rec.normal = vec3(0, 1, 0);
			else if (abs(rec.p.z() - z0) < 0.001)
				rec.normal = vec3(0, 0, -1);
			else if (abs(rec.p.z() - z1) < 0.001)
				rec.normal = vec3(0, 0, 1);
			else
				lanlog::log_error("box normal calculation error.");
			return true;
		}
	}
	
}

bool Box::hitP(const ray& r, float& t) const {
	return false;
}