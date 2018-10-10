#include "Grid.h"

static const float kEpsilon = 0.0001;

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

BBox Grid::get_bounding_box() {
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
						if (counts[index] == 1) {
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
	return false;
}

bool Grid::hitP(const ray& r, float& t) const {
	return false;
}