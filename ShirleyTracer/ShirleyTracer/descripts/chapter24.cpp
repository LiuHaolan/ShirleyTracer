// 24 - 6
World* build() {
	
	int nx = 600;
	int ny = 400;
	int ns = 16;

	World* w = new World;
	vec3 lookfrom(75, 40, 100);
	vec3 lookat(-10, 39, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 360;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new WhittedIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	w->max_depth = 10;
	PointLight* light_ptr = new PointLight(3.0,white,vec3(150,150,0));
	light_ptr->set_shadows(true);
	w->add_light(light_ptr);

	// yellow-green reflective sphere

	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.25);
	reflective_ptr1->set_kd(0.5);
	reflective_ptr1->set_cd(0.75, 0.75, 0);    	// yellow
	reflective_ptr1->set_ks(0.15);
	reflective_ptr1->set_exp(100.0);
	reflective_ptr1->set_kr(0.75);
	reflective_ptr1->set_cr(white); 			// default color

	float radius = 23.0;
	sphere* sphere_ptr1 = new sphere(vec3(38, radius, -25), radius);
	sphere_ptr1->set_material(reflective_ptr1);
	w->add_object(sphere_ptr1);

	// orange non-reflective sphere

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.45);
	matte_ptr1->set_kd(0.75);
	matte_ptr1->set_cd(0.75, 0.25, 0);   // orange

	sphere* sphere_ptr2 = new sphere(vec3(-7, 10, 42), 20);
	sphere_ptr2->set_material(matte_ptr1);
	w->add_object(sphere_ptr2);


	// sphere on top of box

	Reflective* reflective_ptr2 = new Reflective;
	reflective_ptr2->set_ka(0.35);
	reflective_ptr2->set_kd(0.75);
	reflective_ptr2->set_cd(black);
	reflective_ptr2->set_ks(0.0);		// default value
	reflective_ptr2->set_exp(1.0);		// default value, but irrelevant in this case
	reflective_ptr2->set_kr(0.75);
	reflective_ptr2->set_cr(white);

	sphere* sphere_ptr3 = new sphere(vec3(-30, 59, 35), 20);
	sphere_ptr3->set_material(reflective_ptr2);
	w->add_object(sphere_ptr3);


	// cylinder

	Reflective* reflective_ptr3 = new Reflective;
	reflective_ptr3->set_ka(0.35);
	reflective_ptr3->set_kd(0.5);
	reflective_ptr3->set_cd(0, 0.5, 0.75);   // cyan
	reflective_ptr3->set_ks(0.2);
	reflective_ptr3->set_exp(100.0);
	reflective_ptr3->set_kr(0.75);
	reflective_ptr3->set_cr(white);

	double bottom = 0.0;
	double top = 85;
	double cylinder_radius = 22;
	SolidCylinder* cylinder_ptr = new SolidCylinder(bottom, top, cylinder_radius);
	cylinder_ptr->set_material(reflective_ptr3);
	w->add_object(cylinder_ptr);


	// box

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.15);
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(0.75, 1.0, 0.75);   // light green

	Box* box_ptr = new Box(vec3(-35, 0, -110), vec3(-25, 60, 65));
	box_ptr->set_material(matte_ptr2);
	w->add_object(box_ptr);



	// ground plane

	PlaneChecker* checker_ptr = new PlaneChecker;
	checker_ptr->set_size(20.0);
	checker_ptr->set_outline_width(2.0);
	checker_ptr->set_color1(white);
	checker_ptr->set_color2(white);
	checker_ptr->set_outline_color(black);

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.30);
	sv_matte_ptr->set_kd(0.9);
	sv_matte_ptr->set_cd(checker_ptr);

	Plane* plane_ptr = new Plane(vec3(0), vec3(0, 1, 0));
	plane_ptr->set_material(sv_matte_ptr);
	w->add_object(plane_ptr);

	w->background_color = vec3(0.15);

	return w;
}
