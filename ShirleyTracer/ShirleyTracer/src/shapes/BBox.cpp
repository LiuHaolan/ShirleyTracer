#include "BBox.h"
#include "..\utility.h"

bool BBox::hit(const ray& r) const {
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
		if (tmin < 0 || tmin >MAXFLOAT)
			return false;
		else {
			return true;
		}
	}
}