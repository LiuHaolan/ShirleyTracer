
// spherical image mapping
World* build() {
	
	int nx = 700;
	int ny = 700;
	int ns = 32;

	World* w = new World;
	vec3 lookfrom(0, 0, 65);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = (lookfrom-lookat).length();
	float aperture = 0.0;
	float distance = 6100;

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

	//DirectionLight* light_ptr1 = new DirectionLight;
	//light_ptr1->set_direction(vec3(-0.25, 0.4, 1));
	//light_ptr1->scale_radiance(5.5);
	//w->add_light(light_ptr1);

	// image:					

	Image* image_ptr = new Image;
	image_ptr->read_file("EarthHighRes.ppm");
//	image_ptr->read_file("earthmap1k.jpg");

	

	// mapping:
	SphericalMap* map_ptr = new SphericalMap;


	// image based texture:  

	ImageTexture* texture_ptr = new ImageTexture;
	texture_ptr->set_image(image_ptr);
	texture_ptr->set_mapping(map_ptr);


	// textured material:

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->set_ka(0.2);
	sv_matte_ptr->set_kd(0.8);
	sv_matte_ptr->set_cd(texture_ptr);

	Matte* matte_ptr = new Matte;
	matte_ptr->set_ka(0.2);
	matte_ptr->set_kd(0.8);
	matte_ptr->set_cd(vec3(1.0,1.0,1.0));

	// generic sphere:

	sphere*	sphere_ptr = new sphere;
	sphere_ptr->set_material(sv_matte_ptr);
//	w->add_object(sphere_ptr);

	// rotated sphere

	Instance* earth_ptr = new Instance(sphere_ptr);
	earth_ptr->rotate_y(60);
	w->add_object(earth_ptr);


	w->background_color = vec3(0.0, 0, 0);

	return w;
}