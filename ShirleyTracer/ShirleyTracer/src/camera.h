#pragma once
#include "ray.h"
#include "utility.h"

class Camera {
public:
	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov,float aspect,float aperture, float focus_dist) {
		/*lower_left_corner = vec3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0.0, 0.0);
		vertical = vec3(0.0, 2.0, 0.0);
		origin = vec3(0.0, 0.0, 0.0);*/


	//	vec3 u, v, w;
		lens_radius = aperture / 2;

		focus_dist = 1.0;

		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
//		lower_left_corner = vec3(-half_width, -half_height, -1.0);
		lower_left_corner = origin - half_width * u *focus_dist - half_height * v * focus_dist - w * focus_dist;
		horizontal = 2 * half_width*u * focus_dist;
		vertical = 2 * half_height*v * focus_dist;
	}
	ray get_ray(float s, float t) {
//		vec3 rd = lens_radius*random_in_unit_disk();
		vec3 rd = vec3(0, 0, 0);
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical-origin-offset); 
	}

	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	float lens_radius;
	vec3 u, v, w;
};