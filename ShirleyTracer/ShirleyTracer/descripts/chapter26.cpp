
World* build() {

	int nx = 300;
	int ny = 300;
	int ns = 100;

	World* w = new World;
	vec3 lookfrom(27.6, 27.4, -80.0);
	vec3 lookat(27.6, 27.4, 0.0);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 0.0;
	float distance = 400;

	// an temporary method to deal with it
	float vfov = 2 * atan2(200, distance) * 180 / M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new GlobalPathIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.0, vec3(1.0, 1.0, 1.0));

	w->max_depth = 10;
	// the ceiling light - doesn't need samples

	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->set_ce(1.0, 0.73, 0.4);
	emissive_ptr->scale_radiance(100);

	vec3 p0;
	vec3 a, b;
	vec3 normal;

	// box dimensions

	double width = 55.28;   	// x direction
	double height = 54.88;  	// y direction
	double depth = 55.92;	// z direction

	p0 = vec3(21.3, height - 0.001, 22.7);
	a = vec3(0.0, 0.0, 10.5);
	b = vec3(13.0, 0.0, 0.0);
	normal = vec3(0.0, -1.0, 0.0);
	Rectangle* light_ptr = new Rectangle(p0, a, b, normal);
	light_ptr->set_material(emissive_ptr);
	light_ptr->set_sampler(new MultiJittered(ns));
	w->add_object(light_ptr);

	AreaLight* light_ptr1 = new AreaLight;
	light_ptr1->set_Emissive(emissive_ptr);
	light_ptr1->set_object(light_ptr);
	w->add_light(light_ptr1);
	// left wall

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(0.0);
	matte_ptr1->set_kd(0.6);
	matte_ptr1->set_cd(0.57, 0.025, 0.025);	 // red
	matte_ptr1->set_sampler(new MultiJittered(ns));

	p0 = vec3(width, 0.0, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(0.0, height, 0.0);
	normal = vec3(-1.0, 0.0, 0.0);
	Rectangle* left_wall_ptr = new Rectangle(p0, a, b, normal);
	left_wall_ptr->set_material(matte_ptr1);
	w->add_object(left_wall_ptr);


	// right wall

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(0.0);
	matte_ptr2->set_kd(0.6);
	matte_ptr2->set_cd(0.37, 0.59, 0.2);	 // green   from Photoshop
	matte_ptr2->set_sampler(new MultiJittered(ns));

	p0 = vec3(0.0, 0.0, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(0.0, height, 0.0);
	normal = vec3(1.0, 0.0, 0.0);
	Rectangle* right_wall_ptr = new Rectangle(p0, a, b, normal);
	right_wall_ptr->set_material(matte_ptr2);
	w->add_object(right_wall_ptr);


	// back wall

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(0.0);
	matte_ptr3->set_kd(0.6);
	matte_ptr3->set_cd(white);
	matte_ptr3->set_sampler(new MultiJittered(ns));

	p0 = vec3(0.0, 0.0, depth);
	a = vec3(width, 0.0, 0.0);
	b = vec3(0.0, height, 0.0);
	normal = vec3(0.0, 0.0, -1.0);
	Rectangle* back_wall_ptr = new Rectangle(p0, a, b, normal);
	back_wall_ptr->set_material(matte_ptr3);
	w->add_object(back_wall_ptr);


	// floor

	p0 = vec3(0.0, 0.0, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(width, 0.0, 0.0);
	normal = vec3(0.0, 1.0, 0.0);
	Rectangle* floor_ptr = new Rectangle(p0, a, b, normal);
	floor_ptr->set_material(matte_ptr3);
	w->add_object(floor_ptr);


	// ceiling

	p0 = vec3(0.0, height, 0.0);
	a = vec3(0.0, 0.0, depth);
	b = vec3(width, 0.0, 0.0);
	normal = vec3(0.0, -1.0, 0.0);
	Rectangle* ceiling_ptr = new Rectangle(p0, a, b, normal);
	ceiling_ptr->set_material(matte_ptr3);
	w->add_object(ceiling_ptr);


	// the two boxes defined as 5 rectangles each

	// short box

	// top


	Box* short_top_ptr = new Box(vec3(8.2,0,6.5),vec3(24.2,16.5,27.4));
	short_top_ptr->set_material(matte_ptr3);
	w->add_object(short_top_ptr);



	//// tall box
	Box* tall_top_ptr = new Box(vec3(26.5, 0, 24.7), vec3(47.2, 33, 45.5));
	tall_top_ptr->set_material(matte_ptr3);
	w->add_object(tall_top_ptr);
	

	w->background_color = vec3(0.0);

	return w;
}
