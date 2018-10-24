#include "Grid.h"
#include "..\..\config.h"

static const float kEpsilon = 0.0001;
static const float kHugeValue = MAXFLOAT;


vec3 Grid::min_coordinates(void) {
	BBox 	object_box;
	vec3 p0(MAXFLOAT);

	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++) {
		object_box = objects[j]->get_bounding_box();

		if (object_box.p.x() < p0[0])
			p0[0] = object_box.p.x();
		if (object_box.p.y() < p0[1])
			p0[1] = object_box.p.y();
		if (object_box.p.z() < p0[2])
			p0[2] = object_box.p.z();
	}

	p0[0] -= kEpsilon; p0[1] -= kEpsilon; p0[2] -= kEpsilon;

	return (p0);
}


//------------------------------------------------------------------ find_max_bounds

// find the maximum grid coordinates, based on the bounding boxes of the objects

vec3 Grid::max_coordinates(void) {
	BBox object_box;
	vec3 p1(-MAXFLOAT);

	int num_objects = objects.size();

	for (int j = 0; j < num_objects; j++) {
		object_box = objects[j]->get_bounding_box();

		if (object_box.q.x() > p1[0])
			p1[0] = object_box.q.x();
		if (object_box.q.y() > p1[1])
			p1[1] = object_box.q.y();
		if (object_box.q.z() > p1[2])
			p1[2] = object_box.q.z();
	}

	p1[0] += kEpsilon; p1[1] += kEpsilon; p1[2] += kEpsilon;

	return (p1);
}

BBox Grid::get_bounding_box() const {
	return bbox;
}

// you need to call this before using grid geometry!
void Grid::setup_cells() {
	
	vec3 p0 = min_coordinates();
	vec3 p1 = max_coordinates();
	bbox = BBox(p0, p1);

	int num_objects = objects.size();
	float wx = p1.x() - p0.x();
	float wy = p1.y() - p0.y();
	float wz = p1.z() - p0.z();

	assert(wx > 0 && wy > 0 && wz > 0);
	float multiplier = 2.0;
	float s = pow(wx*wy*wz / num_objects, 0.3333333);
	nx = multiplier * wx / s + 1;
	ny = multiplier * wy / s + 1;
	nz = multiplier * wz / s + 1;

	int num_cells = nx * ny*nz;
	cells.resize(num_cells);

	vector<int> counts;
	counts.reserve(num_cells);

	for (int j = 0; j < num_cells; j++)
		counts.push_back(0);

	BBox obj_bBox; 	// object's bounding box
	int index;  	// cell's array index

	for (int j = 0; j < num_objects; j++) {
		obj_bBox = objects[j]->get_bounding_box();

		// compute the cell indices at the corners of the bounding box of the object

		int ixmin = clamp((obj_bBox.p[0] - p0.x()) * nx / (p1.x() - p0.x()), 0, nx - 1);
		int iymin = clamp((obj_bBox.p[1] - p0.y()) * ny / (p1.y() - p0.y()), 0, ny - 1);
		int izmin = clamp((obj_bBox.p[2] - p0.z()) * nz / (p1.z() - p0.z()), 0, nz - 1);
		int ixmax = clamp((obj_bBox.q[0] - p0.x()) * nx / (p1.x() - p0.x()), 0, nx - 1);
		int iymax = clamp((obj_bBox.q[1] - p0.y()) * ny / (p1.y() - p0.y()), 0, ny - 1);
		int izmax = clamp((obj_bBox.q[2] - p0.z()) * nz / (p1.z() - p0.z()), 0, nz - 1);

		// add the object to the cells

		for (int iz = izmin; iz <= izmax; iz++) 					// cells in z direction
			for (int iy = iymin; iy <= iymax; iy++)					// cells in y direction
				for (int ix = ixmin; ix <= ixmax; ix++) {			// cells in x direction
					index = ix + nx * iy + nx * ny * iz;

					if (counts[index] == 0) {
						cells[index] = objects[j];
						counts[index] += 1;  						// now = 1
					}
					else {
						if (counts[index] >= 1) {
							Compound* compound_ptr = new Compound;	// construct a compound object
							compound_ptr->add_objects(cells[index]); // add object already in cell
							compound_ptr->add_objects(objects[j]);  	// add the new object
							cells[index] = compound_ptr;			// store compound in current cell
							counts[index] += 1;  					// now = 2
						}
						else {										// counts[index] > 1
							cells[index] = objects[j];	// just add current object
							counts[index] += 1;						// for statistics only
						}
					}
				}
	}  // end of for (int j = 0; j < num_objects; j++)


	// erase the Compound::vector that stores the object pointers, but don't delete the objects

	objects.erase(objects.begin(), objects.end());


	// display some statistics on counts
	// this is useful for finding out how many cells have no objects, one object, etc
	// comment this out if you don't want to use it

	int num_zeroes = 0;
	int num_ones = 0;
	int num_twos = 0;
	int num_threes = 0;
	int num_greater = 0;

	for (int j = 0; j < num_cells; j++) {
		if (counts[j] == 0)
			num_zeroes += 1;
		if (counts[j] == 1)
			num_ones += 1;
		if (counts[j] == 2)
			num_twos += 1;
		if (counts[j] == 3)
			num_threes += 1;
		if (counts[j] > 3)
			num_greater += 1;
	}

	cout << "num_cells =" << num_cells << endl;
	cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << endl;
	cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << endl;

	// erase the temporary counts vector

	counts.erase(counts.begin(), counts.end());

}


bool Grid::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

	if (turn_off_grid) {
		// turn off the grid, the code
		hit_record tmp;
		float tmin = MAXFLOAT;
		bool flag = false;
		for (int i = 0; i < cells.size(); i++) {
			if (cells[i] && cells[i]->hit(r, t_min, t_max, tmp)) {
				flag = true;
				if (tmp.t < tmin) {
					tmin = tmp.t;
					rec = tmp;
				}
			}
		}
		return flag;
	}

	// the code

	double ox = r.A.x();
	double oy = r.A.y();
	double oz = r.A.z();
	double dx = r.B.x();
	double dy = r.B.y();
	double dz = r.B.z();

	double x0 = bbox.p[0];
	double y0 = bbox.p[1];
	double z0 = bbox.p[2];
	double x1 = bbox.q[0];
	double y1 = bbox.q[1];
	double z1 = bbox.q[2];

	double tx_min = 0, ty_min = 0, tz_min = 0.0;
	double tx_max = 0, ty_max = 0, tz_max = 0.0;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0 = 0, t1 = 0;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);

	// initial cell coordinates

	int ix = 0, iy = 0, iz = 0;

	if (bbox.inside(r.A)) {  			// does the ray start inside the grid?
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		vec3 p = r.point_at_parameter(t0);  // initial hit point with grid's bounding box
		ix = clamp((p.x() - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y() - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z() - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next = 0, ty_next = 0, tz_next = 0.0;
	int 	ix_step = 0, iy_step = 0, iz_step = 0;
	int 	ix_stop = 0, iy_stop = 0, iz_stop = 0;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}


	// traverse the grid

	while (true) {
		hitable* object_ptr = cells[ix + nx * iy + nx * ny * iz];

		////debug code
		//if (object_ptr) {
		//	int q = 112;
		//	int y = 213;
		//}

		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr ) {
				if (object_ptr->hit(r, t_min, t_max, rec) && rec.t < tx_next) {
	//				rec.mat_ptr = object_ptr->get_material();

					return (true);
				}
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr) {
					if (object_ptr->hit(r, t_min, t_max, rec) && rec.t < ty_next) {
						//				rec.mat_ptr = object_ptr->get_material();

						return (true);
					}
				}
				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
				if (object_ptr) {
					if (object_ptr->hit(r, t_min, t_max, rec) && rec.t < tz_next) {
						//				rec.mat_ptr = object_ptr->get_material();

						return (true);
					}
				}
				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}

}	// end of hit

bool Grid::hitP(const ray& r, float& t) const {
	double ox = r.A.x();
	double oy = r.A.y();
	double oz = r.A.z();
	double dx = r.B.x();
	double dy = r.B.y();
	double dz = r.B.z();

	double x0 = bbox.p[0];
	double y0 = bbox.p[1];
	double z0 = bbox.p[2];
	double x1 = bbox.q[0];
	double y1 = bbox.q[1];
	double z1 = bbox.q[2];

	double tx_min = 0 , ty_min = 0, tz_min = 0;
	double tx_max = 0, ty_max = 0, tz_max = 0;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);

	// initial cell coordinates

	int ix = 0, iy = 0, iz = 0;

	if (bbox.inside(r.A)) {  			// does the ray start inside the grid?
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		vec3 p = r.point_at_parameter(t0);  // initial hit point with grid's bounding box
		ix = clamp((p.x() - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y() - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z() - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next = 0, ty_next = 0, tz_next = 0;
	int 	ix_step = 0, iy_step = 0, iz_step = 0;
	int 	ix_stop = 0, iy_stop = 0, iz_stop = 0;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = kHugeValue;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = kHugeValue;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = kHugeValue;
		iz_step = -1;
		iz_stop = -1;
	}


	// traverse the grid

	while (true) {
		hitable* object_ptr = cells[ix + nx * iy + nx * ny * iz];

		////debug code
		//if (object_ptr) {
		//	int q = 112;
		//	int y = 213;
		//}

		if (tx_next < ty_next && tx_next < tz_next) {
			if (object_ptr) {
				if (object_ptr->hitP(r, t) && t < tx_next) {
					//				rec.mat_ptr = object_ptr->get_material();
					return (true);
				}
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop)
				return (false);
		}
		else {
			if (ty_next < tz_next) {
				if (object_ptr && object_ptr->hitP(r,t) && t < ty_next) {
					//				rec.mat_ptr = object_ptr->get_material();
					return (true);
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop)
					return (false);
			}
			else {
				if (object_ptr && object_ptr->hitP(r, t) && t < tz_next) {
					//				rec.mat_ptr = object_ptr->get_material();
					return (true);
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop)
					return (false);
			}
		}
	}
}	//end of hit


Grid::Grid(Mesh* ptr) {

	// you need to set num_triangles when setting the mesh
	for (int j = 0; j < ptr->num_triangles; j++) {
		int a = ptr->vertex_faces[j][0];
		int b = ptr->vertex_faces[j][1];
		int c = ptr->vertex_faces[j][2];

		MeshTriangle* obj_ptr = new MeshTriangle(ptr, a, b, c);
		add_objects(obj_ptr);

	}

	setup_cells();

	mat = ptr->material;
}