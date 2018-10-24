#pragma once

#include <vector>
#include "hitable.h"
#include ".\lights\Ambient.h"
//#include "hit_record.h"
#include "camera.h"
#include "integrator.h"
#include "light.h"
#include "Camera/Pinhole.h"

using namespace std;

class World {
public:
	World() { inside = false; }

	void add_object(hitable* obj);
	void add_light(Light* lit);
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

	Pinhole* ncam_ptr;
	Integrator* integrator_ptr;
	Light* ambient_ptr;

	vector<hitable* > objects;
	vector<Light* > lights;

	vec3 background_color;
	int max_depth;


	// be careful!
	static bool inside;
};