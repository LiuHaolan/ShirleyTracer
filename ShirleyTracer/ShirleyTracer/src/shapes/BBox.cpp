#include "BBox.h"
#include "..\utility.h"

BBox surrounding_box(BBox b1, BBox b2) {
	vec3 small(min(b1.p.x(), b2.p.x()), min(b1.p.y(), b2.p.y()), min(b1.p.z(), b2.p.z()));
	vec3 large(max(b1.q.x(), b2.q.x()), max(b1.q.y(), b2.q.y()), max(b1.q.z(), b2.q.z()));

	return BBox(small, large);
}

bool BBox::hit(const ray& r, float t_min, float t_max) const {
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

	double tmin = max(max(min(tx1, tx2), min(ty1, ty2)), min(tz1, tz2));
	double tmax = min(min(max(tx1, tx2), max(ty1, ty2)), max(tz1, tz2));

	if (tmax < tmin) {
		return false;
	}
	else {
		if (tmin < t_min || tmax >t_max)
			return false;
		else {
			return true;
		}
	}
}

bool BBox::inside(vec3 o)const {
	if (o.x() < p.x() || o.x() > q.x())
		return false;
	if (o.y() < p.y() || o.y() > q.y())
		return false;
	if (o.z() < p.z() || o.z() > q.z())
		return false;
	return true;
}