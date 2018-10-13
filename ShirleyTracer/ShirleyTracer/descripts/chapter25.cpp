
World* build() {
	
	int nx = 600;
	int ny = 600;
	int ns = 25;

	World* w = new World;
	vec3 lookfrom(-150, 75, 500);
	vec3 lookat(-6, 50, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 2000;

	// an temporary method to deal with it
	float vfov = 2*atan2(200,distance)*180/M_PI;
	// default up vector vec3(0,1,0)
	Camera* c = new Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus);
	w->camera_ptr = c;
	w->integrator_ptr = new AreaLightIntegrator(w);
	w->nx = nx;
	w->ny = ny;
	w->ns = ns;

	w->ambient_ptr = new Ambient_Light(0.5,vec3(1.0,1.0,1.0));

	w->max_depth = 1;
	PointLight* light_ptr = new PointLight(2.0,white,vec3(250,500,250));
	light_ptr->set_shadows(false);
	w->add_light(light_ptr);

	// yellow-green reflective sphere

//	float exp = 1.0;  		// for Figure 25.30(a)
//	float exp = 10.0;  		// for Figure 25.30(b)
//	float exp = 100.0;  	// for Figure 25.30(c)
//	float exp = 1000.0;  	// for Figure 25.30(d)
	float exp = 10000.0;  	// for Figure 25.30(e)
//	float exp = 100000.0;  	// for Figure 25.30(f)

	GlossyReflective* glossy_ptr = new GlossyReflective;
	glossy_ptr->set_samples(ns, exp);
	glossy_ptr->set_ka(0.0);
	glossy_ptr->set_kd(0.0);
	glossy_ptr->set_ks(0.0);	// 0.0 lhl
	glossy_ptr->set_exp(exp);
	glossy_ptr->set_cd(1.0, 1.0, 0.3);
	glossy_ptr->set_kr(0.9);
	glossy_ptr->set_exponent(exp);
	glossy_ptr->set_cr(vec3(1.0, 1.0, 0.3));  // lemon

	sphere* sphere_ptr1 = new sphere(vec3(-6, 55, 0), 40);
	sphere_ptr1->set_material(glossy_ptr);
	w->add_object(sphere_ptr1);

	//cylinder

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0.15);
	matte_ptr->set_kd(0.75);
	matte_ptr->set_cd(0.5, 1.0, 0.5);   // green

	double bottom = -100;
	double top = 15;
	double radius = 30;
	Instance* cylinder_ptr = new Instance(new SolidCylinder(bottom, top, radius));
	cylinder_ptr->translate(-6, 0, 0);
	cylinder_ptr->set_material(matte_ptr);
	w->add_object(cylinder_ptr);

	Image* image_ptr = new Image;
	image_ptr->read_file("uffizi_probe_small.ppm");  // for testing 
	LightProbe* light_probe_ptr = new LightProbe;
	light_probe_ptr->set_map_type(light_prob);
	ImageTexture* image_texture_ptr = new ImageTexture(image_ptr);
	image_texture_ptr->set_mapping(light_probe_ptr);

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(1);
	sv_matte_ptr->set_kd(0.85);
	sv_matte_ptr->set_cd(image_texture_ptr);

	sphere* unit_sphere_ptr = new sphere;
//	unit_sphere_ptr->set_shadows(false);

	Instance* sphere_ptr2 = new Instance(unit_sphere_ptr);
	sphere_ptr2->scale(1000000.0);
	sphere_ptr2->set_material(sv_matte_ptr);
	sphere_ptr2->transform_texture(true);

	w->add_object(sphere_ptr2);

	w->background_color = vec3(0.15);

	return w;
}