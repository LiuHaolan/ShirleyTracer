#pragma once

#include <vector>
#include "hitable.h"
#include ".\lights\Ambient.h"
//#include "shaderec.h"
#include "camera.h"
#include "integrator.h"
#include "light.h"

using namespace std;

class World {
public:
	void add_object(hitable* obj);
	bool hit(const ray& r, float tmin,float tmax, hit_record& sr);
	~World() {
		delete ambient_ptr;
		delete camera_ptr;
		delete integrator_ptr;
	}

	int nx;
	int ny;
	int ns;
	Camera* camera_ptr;
	Integrator* integrator_ptr;
	Light* ambient_ptr;

	vector<hitable* > objects;
	vector<Light* > lights;
};