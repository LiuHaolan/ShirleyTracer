
#include "ImageTexture.h"

// ---------------------------------------------------------------- default constructor

ImageTexture::ImageTexture(void)
	:	Texture(),
		hres(100),
		vres(100),
		image_ptr(NULL),
		mapping_ptr(NULL)
{}


// ---------------------------------------------------------------- constructor

ImageTexture::ImageTexture(Image* _image_ptr)
	:	Texture(),
		hres(_image_ptr->get_hres()),
		vres(_image_ptr->get_vres()),
		image_ptr(_image_ptr),
		mapping_ptr(NULL)
{}


ImageTexture::~ImageTexture (void) {

	if (image_ptr) {
		delete image_ptr;
		image_ptr = NULL;
	}
	
	if (mapping_ptr) {
		delete mapping_ptr;
		mapping_ptr = NULL;
	}
}


// ---------------------------------------------------------------- get_color

// When we ray trace a triangle mesh object with uv mapping, the mapping pointer may be NULL
// because we can define uv coordinates on an arbitrary triangle mesh.
// In this case we don't use the local hit point because the pixel coordinates are computed 
// from the uv coordinates stored in the hit_record object in the uv triangles' hit functions
// See, for example, Listing 29.12.

vec3														
ImageTexture::value(const hit_record& sr) const {	
	int row;
	int column;
		
	if (mapping_ptr)
		mapping_ptr->get_texel_coordinates(sr.local_hit_point, hres, vres, row, column);
	else {
		float v = abs(sr.v - floor(sr.v));
		float u = abs(sr.u - floor(sr.u));
		row 	= (int)(v * (vres - 1));  	
		column 	= (int)(u * (hres - 1));	
	}
	
	return (image_ptr->get_color(row, column));
}  