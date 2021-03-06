#pragma once
#include "..\..\vec3.h"
#include <vector>
#include "..\..\materials.h"
using namespace std;
const static bool smooth = false;

class Mesh {
public:
	Mesh(){
		has_normals = false;
	}
	void read_file(const char* filename);
	void setup_smooth();

	vector<vec3> 		vertices;				// mesh vertices 
	vector<vec3> 			normals;				// average normal at each vertex;
	vector<float>			u;						// u texture coordinate at each vertex
	vector<float>			v;						// v texture coordinate at each vertex
	vector<vector<int>>			faceuv_indices;

	vector<vector<int>>			facenormal_indices;
	vector<vec3>		naive_normals;
	bool has_normals;

	vector<vector<int> > 	vertex_faces;			// the triangles shared by each vertex
	int 					num_vertices; 			// number of vertices
	int 					num_triangles; 			// number of triangles
	
	
	Material* material;
	void set_mesh_material(Material* ptr) { material = ptr; }

private:
	//// ban the copy constructor
	//Mesh( Mesh& m) {

	//}

	//Mesh(const Mesh& m){
	//	
	//}

	//// ban the use of operator=
	//Mesh& operator= (const Mesh& rhs) {
	//	return *this;
	//}
};
