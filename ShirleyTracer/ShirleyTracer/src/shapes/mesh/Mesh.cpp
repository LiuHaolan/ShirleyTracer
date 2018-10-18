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

	// record each vertices 
	std::vector<vector<int>> vertex_neif;
	vertex_neif.resize(vertices.size());

	normals.resize(vertices.size());
	for (int j = 0; j < faces.size(); j++) {
		vector<int> tmp;
		tmp.push_back(faces[j].x);
		tmp.push_back(faces[j].y);
		tmp.push_back(faces[j].z);
		vertex_faces[j] = tmp;

		////debug
		//if (faces[j].x == 8 || faces[j].y == 8 || faces[j].z == 8)
		//	assert(false);

		vertex_neif[faces[j].x].push_back(j);
		vertex_neif[faces[j].y].push_back(j);
		vertex_neif[faces[j].z].push_back(j);
	}

	if(smooth){
	int cnt = 0;
	for (int i = 0; i < vertex_neif.size(); i++) {
		vec3 normalsum = 0;

		vector<int>& neigh_faces = vertex_neif[i];
		int num_faces = neigh_faces.size();

		//		assert(num_faces>0);
		if (num_faces == 0) {
			cnt++;
			continue;
		}

		int vid1 = vertex_faces[neigh_faces[0]][0];
		int vid2 = vertex_faces[neigh_faces[0]][1];
		int vid3 = vertex_faces[neigh_faces[0]][2];
		vec3 norm = 0;
		int next;
		int for_ind;

		if (vid1 == i) {
			// please, vec3 here
			norm = cross(vertices[vid2] - vertices[i], vertices[vid3] - vertices[i]);
			next = vid3;
		}
		else if (vid2 == i) {
			norm = cross(vertices[vid1] - vertices[i], vertices[vid3] - vertices[i]);
			next = vid3;
		}
		else {
			norm = cross(vertices[vid2] - vertices[i], vertices[vid1] - vertices[i]);
			next = vid1;
		}
		for_ind = 0;
		normalsum = normalsum + unit_vector(norm);

		int k = 0;
		while (k++ < num_faces - 1) {
			// search the face with the next
			int theleft = 0;
			int j = 0;
			for (j = 0; j < num_faces; j++) {
				// don't search the before mentioned edge.
				if (j == for_ind)
					continue;

				int vid1_ = vertex_faces[neigh_faces[j]][0];
				int vid2_ = vertex_faces[neigh_faces[j]][1];
				int vid3_ = vertex_faces[neigh_faces[j]][2];
				if (next == vid1_) {
					if (vid2_ == i)
						theleft = vid3_;
					else
						theleft = vid2_;
					break;
				}
				if (next == vid2_) {
					if (vid1_ == i)
						theleft = vid3_;
					else
						theleft = vid1_;
					break;
				}
				if (next == vid3_) {
					if (vid1_ == i)
						theleft = vid2_;
					else
						theleft = vid1_;
					break;
				}
			}

			//set for_ind
			for_ind = j;

			// three point, i,next
			norm = cross((vertices[i] - vertices[next]), (vertices[i] - vertices[theleft]));
			normalsum = normalsum + unit_vector(norm);
			// set next
			next = theleft;
		}

		normals[i] = normalsum / num_faces;
	}

	std::cout << cnt << std::endl;
}
}