

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <stdio.h>

#include "..\utility.h"   // defines red
#include "Image.h"

#include "..\lanlog.h"

// ---------------------------------------------------- default constructor

Image::Image(void)
	:	hres(100),
		vres(100)
{}


// ---------------------------------------------------- copy constructor								

Image::Image(const Image& image)
	:	hres(image.hres),
		vres(image.vres),
		pixels(image.pixels)
{}		


// ---------------------------------------------------- assignment operator	

Image& 										
Image::operator= (const Image& rhs) {
	if (this == &rhs)
		return (*this);
	
	hres 		= rhs.hres;
	vres 		= rhs.vres;
	pixels 		= rhs.pixels;

	return (*this);
}		

// ---------------------------------------------------- destructor	

Image::~Image(void)	{}
	

// ---------------------------------------------------- read_ppm_file

void										
Image::read_file(const char* file_name) {
	int nn;
	unsigned char* tex_data = stbi_load(file_name, &hres, &vres, &nn, 0);
	pixels.resize(hres*vres);
	for (int i = 0; i < hres*vres; i++) {
		pixels[i] = vec3(int(tex_data[3*i])*1.0 / 255.0, int(tex_data[3*i + 1])*1.0 / 255.0, int(tex_data[3*i + 2])*1.0 / 255.0);
//		lanlog::log_vec3(pixels[i]);
	}
}



// --------------------------------------------------------------------------------------------- get_color 

vec3									
Image::get_color(const int row, const int column) const {
	int index = column + hres * (row );
	int pixels_size = pixels.size();
	
	if (index < pixels_size)
		return (pixels[index]);
	else {
//		lanlog::log_info("image texture color index overflow.");
		return (vec3(1.0, 0, 0));    // useful for debugging 
	}
		
}

 
