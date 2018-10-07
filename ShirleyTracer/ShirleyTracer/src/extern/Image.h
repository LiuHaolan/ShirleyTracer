#ifndef __IMAGE__
#define __IMAGE__

// This is not a texture
// The Image class stores an image in PPM format
// This is used for image based textures, including ramp textures such as marble and sandstone
// I originally chose the PPM format because it's simple to parse


#include <vector>		
#include "..\vec3.h"


using namespace std;

//--------------------------------------------------------------------- class Image

class Image {	
	public:
	
		Image(void);								

		Image(const Image& image);					

		Image& 										
		operator= (const Image& image);		

		~Image(void) ;								
		
		void										
		read_file(const char* file_name);
		
		int
		get_hres(void);	
		
		int
		get_vres(void);	
				
		vec3									
		get_color(const int row, const int col) const;		
		
	private:
		int 				hres;			// horizontal resolution of image
		int					vres;			// vertical resolution of image
		vector<vec3> 	pixels;
};


//--------------------------------------------------------------------- get_hres

inline int
Image::get_hres(void) {
	return (hres);
}


//--------------------------------------------------------------------- get_vres

inline int
Image::get_vres(void) {
	return (vres);
}

#endif
		
