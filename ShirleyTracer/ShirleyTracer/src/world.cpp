#include "World.h"

void World::add_object(hitable* obj) {
	objects.push_back(obj);
}