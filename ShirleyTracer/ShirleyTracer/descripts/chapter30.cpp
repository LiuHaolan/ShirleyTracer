// 30.02
World* build() {
	
	int nx = 880;
	int ny = 300;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(100, 100, 300);
	vec3 lookat(-0.2, 1.0, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 10000;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new RayCastIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(4.5,vec3(1.0,1.0,1.0));

	DirectionLight* light_ptr1 = new DirectionLight;
	light_ptr1->set_direction(vec3(0, 0, 1));
	light_ptr1->scale_radiance(1.5);
	w->add_light(light_ptr1);

	Checker3D* checker_pt1 = new Checker3D;
	checker_pt1->set_size(1.0);
	checker_pt1->set_color1(black);
	checker_pt1->set_color2(white);


	// textured material:

	SV_Matte* sv_matte_ptr1 = new SV_Matte;
	sv_matte_ptr1->set_ka(0.75);
	sv_matte_ptr1->set_kd(0.75);
	sv_matte_ptr1->set_cd(checker_pt1);

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0.2);
	matte_ptr->set_kd(0.8);
	matte_ptr->set_cd(vec3(1.0,1.0,1.0));

	// generic sphere:

	sphere* sphere_ptr = new sphere(vec3(-9.5, -1, 0), 2.5);
	sphere_ptr->set_material(sv_matte_ptr1);
	w->add_object(sphere_ptr);

	Checker3D* checker_ptr2 = new Checker3D;
	checker_ptr2->set_size(1.0);
	checker_ptr2->set_color1(black);
	checker_ptr2->set_color2(white);

	SV_Matte* sv_matte_ptr2 = new SV_Matte;
	sv_matte_ptr2->set_ka(0.75);
	sv_matte_ptr2->set_kd(0.75);
	sv_matte_ptr2->set_cd(checker_ptr2);

	Instance* cylinder_ptr = new Instance(new cylinder(-2.5, 4.0, 1.95));
	cylinder_ptr->translate(-3.5, 0, 0);
	cylinder_ptr->transform_texture(false);
	cylinder_ptr->set_material(sv_matte_ptr2);
	w->add_object(cylinder_ptr);

	w->background_color = vec3(0.5, 0.5, 0.5);

	return w;
}