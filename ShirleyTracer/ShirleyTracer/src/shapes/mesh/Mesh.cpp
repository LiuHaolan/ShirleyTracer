#include "Mesh.h"
#include "..\..\plyparser.h"


void Mesh::read_file(const char* filename) {
	std::vector<float3> verts;
	std::vector<uint3> faces;

	read_ply_file(filename, verts, faces);
	vertices.resize(verts.size());
	num_vertices = verts.size();
	for (int i = 0; i < verts.size(); i++) {
		vertices[i] = vec3(verts[i].x, verts[i].y, verts[i].z);
	}
	vertex_faces.resize(faces.size());
	num_triangles = faces.size();
	for (int j = 0; j < faces.size(); j++) {
		vector<int> tmp;
		tmp.push_back(faces[j].x);
		tmp.push_back(faces[j].y);
		tmp.push_back(faces[j].z);
		vertex_faces[j] = tmp;
	}

}