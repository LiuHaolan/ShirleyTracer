#include "MeshAggregate.h"


MeshAggregate::MeshAggregate(Mesh* ptr) {

	// you need to set num_triangles when setting the mesh
	for (int j = 0; j < ptr->num_triangles; j++) {
		int a = ptr->vertex_faces[j][0];
		int b = ptr->vertex_faces[j][1];
		int c = ptr->vertex_faces[j][2];

		MeshTriangle* obj_ptr = new MeshTriangle(ptr, a, b, c);
		add_objects(obj_ptr);

	}

	mat = ptr->material;
}