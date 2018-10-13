#include "LightProbe.h"
#include "..\utility.h"
void LightProbe::get_texel_coordinates(const 	vec3& 	hit_point,
	const 	int 		xres,
	const 	int 		yres,
	int& 		row,
	int& 		column) const 
{
	float x = hit_point.x();
	float y = hit_point.y();
	float z = hit_point.z();

	float d = sqrt(x*x + y * y);
	float sin_beta = y / d;
	float cos_beta = x / d;
	float alpha;

	if (maptype == light_prob)
		alpha = acos(z);
	if (maptype == panoramic)
		alpha = acos(-z);

	float r = alpha * INV_PI;
	float u = (1.0 + r * cos_beta)*0.5;
	float v = (1.0 + r * sin_beta) * 0.5;
	column = (int)((xres - 1)*u);
	row = (int)((yres - 1)*v);
}