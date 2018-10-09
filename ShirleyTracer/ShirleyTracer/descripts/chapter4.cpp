
World* build() {
	
	int nx = 300;
	int ny = 200;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(100, 500, 100);
	vec3 lookat(0, 450.0, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 175;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new RayCastIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(1.0,vec3(1.0,1.0,1.0));

	DirectionLight* light_ptr1 = new DirectionLight;
	light_ptr1->set_direction(vec3(0, 1, 0));
	light_ptr1->scale_radiance(4.0);
	w->add_light(light_ptr1);

	Checker3D* checker_pt1 = new Checker3D;
	checker_pt1->set_size(250.0);
	checker_pt1->set_color1(black);
	checker_pt1->set_color2(white);


	// textured material:

	SV_Matte* sv_matte_ptr1 = new SV_Matte;
	sv_matte_ptr1->set_ka(0.5);
	sv_matte_ptr1->set_kd(0.35);
	sv_matte_ptr1->set_cd(checker_pt1);

	Plane* plane_ptr = new Plane(vec3(0.0), vec3(0.0, 1.0, 0.0));
	plane_ptr->set_material(sv_matte_ptr1);
	w->add_object(plane_ptr);

	w->background_color = vec3(0.25, 0.25, 0.25);

	return w;
}