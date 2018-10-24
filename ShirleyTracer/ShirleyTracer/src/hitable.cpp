#include "hitable.h"
#include "shapes/BBox.h"

BBox hitable::get_bounding_box() const {
	lanlog::log_error("BBox base function called!");
	return BBox();
}