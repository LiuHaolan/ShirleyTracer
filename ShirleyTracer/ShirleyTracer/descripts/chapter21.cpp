
World* build() {
	
	int nx = 400;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(100, 0, 100);
	vec3 lookat(0, 1, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 6000;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new RayCastIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	PointLight* light_ptr2 = new PointLight(3.0,vec3(1.0,1.0,1.0), vec3(50, 50, 1));
	w->add_light(light_ptr2);

	Phong* phong_ptr = new Phong;
	phong_ptr->set_cd(0.75);
	phong_ptr->set_ka(0.25);
	phong_ptr->set_kd(0.8);
	phong_ptr->set_ks(0.15);
	phong_ptr->set_exp(50.0);

	Instance* ellipsoid_ptr = new Instance(new sphere);
	ellipsoid_ptr->set_material(phong_ptr);
	ellipsoid_ptr->scale(2, 3, 1);
	ellipsoid_ptr->rotate_x(-45);
	ellipsoid_ptr->translate(0, 1, 0);
	w->add_object(ellipsoid_ptr);
	sphere* sphere_ptr = new sphere;
	sphere_ptr->set_material(phong_ptr);
	w->add_object(sphere_ptr);


	w->background_color = vec3(0.0, 0, 0);

	return w;
}