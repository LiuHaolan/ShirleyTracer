#pragma once
#include "..\texture.h"
#include "..\extern\Image.h"
#include "mapping.h"

class ImageTexture : public Texture {
public:

	ImageTexture(void);

	ImageTexture(Image* _image_ptr);

	virtual
		~ImageTexture(void);

	virtual vec3
		value(const hit_record& sr) const;

	void
		set_image(Image* _image_ptr);

	void
		set_mapping(Mapping* map_ptr);

private:

	int 		hres;			// horizontal resolution of the image
	int			vres;			// vertical resolution of the image
	Image*		image_ptr;		// the image
	Mapping*	mapping_ptr;	// mapping technique used, if any
};


inline void
ImageTexture::set_image(Image* _image_ptr) {
	image_ptr = _image_ptr;
	hres = image_ptr->get_hres();
	vres = image_ptr->get_vres();
}


// ---------------------------------------------------------------- set_mapping

inline void
ImageTexture::set_mapping(Mapping* map_ptr) {
	mapping_ptr = map_ptr;
}