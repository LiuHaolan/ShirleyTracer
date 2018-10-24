#define _CRT_SECURE_NO_WARNINGS

#include "parsing.h"
#include "shapes/MeshAggregate.h"
#include "shapes/accelerator/BVH.h"

struct transform_global* transform_st;
struct viewParams* mViewParams;
struct AnimationParams* mAnimationParams;
struct AnimationList* mAnimations;
float g_amb[3];
float g_diff[3];
float g_spec[3];
float g_Shine;
float g_T;
float g_index_of_refraction;

static int gDetailLevel = 0;  /* zero as default */
static const std::string prefix_dir = "./kitchen/";

static void eatWhitespace(FILE *f)
{
	char ch = getc(f);
	while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\f' || ch == '\r')
		ch = getc(f);
	ungetc(ch, f);
}

static void parseComment(FILE *f)
{
	char str[1000];
	fgets(str, 1000, f);
}

static void parseViewpoint(FILE *fp, World* ptr)
{
	vec3 from;
	vec3 at;
	vec3 up;
	float fov_angle;		// you read some degree.
	float hither;
	int resx;
	int resy;


	if (fscanf(fp, " from %f %f %f", &from[0], &from[1], &from[2]) != 3) {
		printf("Parser view syntax error");
		return;
	}


	if (fscanf(fp, " at %f %f %f", &at[0], &at[1], &at[2]) != 3) {
		printf("Parser view syntax error");
		return;
	}

	if (fscanf(fp, " up %f %f %f", &up[0], &up[1], &up[2]) != 3) {
		printf("Parser view syntax error");
		return;
	}

	if (fscanf(fp, " angle %f", &fov_angle) != 1) {
		printf("Parser view syntax error");
		return;
	}

	if (fscanf(fp, " hither %f", &hither) != 1) {
		printf("Parser view syntax error");
		return;
	}

	if (hither < 0.0001) hither = 1.0f;

	if (fscanf(fp, " resolution %d %d", &resx, &resy) != 2) {
		printf("Parser view syntax error");
		return;
	}

	/* init your view point here:
	 * e.g, viInitViewpoint(from, at, up, fov_angle, hither, resx, resy);
	 */
	
//	ptr->camera_ptr = new Camera(from, at, up, fov_angle, float(resx) / float(resy), aperture, dist_to_focus);
	mViewParams = new viewParams;
	mViewParams->at = at;
	mViewParams->fov_angle = fov_angle;
	mViewParams->from = from;
	mViewParams->hither = hither;
	mViewParams->resx = resx;
	mViewParams->resy = resy;
	mViewParams->up = up;

	return;

}

static void parseLight(FILE *fp, World* ptr)
{
	vec3 pos;
	vec3 col;
	int num;
	int is_animated;
	char name[100];
	strcpy_s(name, "");

	is_animated = getc(fp);
	if (is_animated != 'a')
	{
		ungetc(is_animated, fp);
		is_animated = 0;
	}

	if (is_animated)  /* if it's an animated light, read its name */
	{
		fscanf(fp, "%s", name);
	}

	if (fscanf(fp, "%f %f %f ", &pos[X], &pos[Y], &pos[Z]) != 3)
	{
		printf("Light source position syntax error");
		system("pause");
		exit(1);
	}

	/* read optional color of light */
	num = fscanf(fp, "%f %f %f ", &col[X], &col[Y], &col[Z]);
	if (num == 0)
	{
		for (int i = 0; i < 3; i++)
			col[i] = 1.0f;
		//col=(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (num != 3)
	{
		printf("Light source color syntax error");
		exit(1);
	}

	/* add light source here:
	 * e.g. viAddLight(name,pos,col);
	 */
	PointLight* light_ptr = new PointLight(1.0, col, pos);
	/*if (is_animated!=0) {
		dynamic_lights_ind.push_back(ptr->lights.size());
		dynamic_lights_name.push_back(name);
	}*/
	ptr->lights.push_back(light_ptr);

}

static void parseBackground(FILE *fp, World* ptr)
{
	vec3 bgcolor;

	if (fscanf(fp, "%f %f %f", &bgcolor[X], &bgcolor[Y], &bgcolor[Z]) != 3)
	{
		printf("background color syntax error");
		system("pause");
		exit(1);
	}

	/* init you background color here
	 * e.g.  viInitBackgroundColor(bgcolor);
	 */
	ptr->background_color = bgcolor;
}

static void parseSphere(FILE *fp, World* ptr)
{
	float radius;
	Vec3f center;

	if (fscanf(fp, "%f %f %f %f", &center[X], &center[Y], &center[Z], &radius) != 4)
	{
		printf("sphere syntax error");
		system("pause");
		exit(1);
	}
	/* add a sphere here
	 * e.g., viAddSphere(center,radius);
	 */
	sphere* sp = new sphere(vec3(center[X], center[Y], center[Z]), radius);
	ptr->add_object(sp);

}



static void parseDetailLevel(FILE *fp)
{
	if (fscanf(fp, "%d", &gDetailLevel) != 1)
	{
		printf("Error: could not parse detail level.\n");
		system("pause");
		exit(1);
	}
}

static void parseInclude(FILE *fp, World* ptr)
{
	char filename[100];
	FILE *ifp;
	int detail_level;
	if (fscanf(fp, "%d %s", &detail_level, filename, 99) != 2)
	{
		printf("Error: could not parse include.\n");
		exit(0);
	}


	if (detail_level <= gDetailLevel) /* skip file if our detail is less than the global detail */
	{
		std::string s = std::string(filename);
		s = prefix_dir+s;

		fopen_s(&ifp, s.c_str(), "r");
		if (ifp)
		{
			viParseFile(ifp,ptr);  /* parse the file recursively */
			fclose(ifp);
		}
		else
		{
			printf("Error: could not open include file: <%s>.\n", filename);
			system("pause");
			exit(1);
		}
	}
	else
	{
		printf("Skipping include file: %s\n", filename);
	}
}

static void parseAnimParams(FILE *fp)
{
	float start, end;
	int num_frames;
	if (fscanf(fp, "%f %f %d", &start, &end, &num_frames) != 3)
	{
		printf("Error: could not parse animations parameters.\n");
		exit(1);
	}
	/* add animations parameters here
	 * e.g., viSetupAnimParams(start,end,num_frames);
	 */
	mAnimationParams = (struct AnimationParams*) calloc(1, sizeof(struct AnimationParams));
	mAnimationParams->start = start;
	mAnimationParams->end = end;
	mAnimationParams->key_nums = num_frames;
}

static void parseA(FILE *f, World* ptr)
{
	char name[100];
	char ch;
	int is_ambient;

	is_ambient = getc(f);
	if (is_ambient != 'm')
	{
		ungetc(is_ambient, f);
		is_ambient = 0;
	}

	if (is_ambient)  /* we got "am" = global ambient light */
	{
		Vec3f amb;
		if (fscanf(f, "%f %f %f", &amb[0], &amb[1], &amb[2]) != 3)
		{
			printf("Error: could not parse ambient light (am).\n");
			system("pause");
			exit(1);
		}

		/* set up your globabl ambient light here using amb */
		//lhl
		ptr->ambient_ptr = new Ambient_Light(1.0,vec3(amb[0], amb[1], amb[2]));
	}
	else
	{
		parseAnimParams(f);
	}
}

static void parseKeyFrames(FILE* fp, World* ptr) {
	char name[200];
	char motion[200];
	char ch;
	int  c;
	int visibility;
	int  ret, i, nKeyFrames;
	float time, x, y, z, angle, te, co, bi;
	PositionKey* PKeys;
	RotationKey* RKeys;
	Animation* animation = new Animation;
	struct AnimationList* animationlist;

	if (fscanf(fp, "%s", name) != 1)
	{
		printf("Error: could not read name of animation.\n");
		system("pause");
		exit(1);
	}
	eatWhitespace(fp);
	ch = getc(fp);
	if (ch != '{')
	{
		printf("Error: could not find a { in animation %s.\n", name);
		system("pause");
		exit(1);
	}

	/* insert a new animation in the AnimationList */
	animationlist =
		(struct AnimationList*) calloc(1, sizeof(struct AnimationList));

	/* put the newly allocated a list somewhere,
	 * e.g.,
	 * animationlist->next = gScene.mAnimations;
	 * gScene.mAnimations = animationlist;
	 * animation = &(animationlist->animation);
	 * gScene.mAnimations was our global list of animations
	 */
	animationlist->next = mAnimations;
	mAnimations = animationlist;
	animation = &(animationlist->animation);


	animation->translations = NULL;
	animation->rotations = NULL;
	animation->scales = NULL;
	animation->name = (char *)malloc(sizeof(name));
	strcpy(animation->name,name);

	eatWhitespace(fp);
	while ((c = getc(fp)) != '}')
	{
		ungetc(c, fp);
		if (fscanf(fp, "%s %d", motion, &nKeyFrames) != 2)
		{
			printf("Error: could not read name of motion or number of keyframes for animation.\n");
			system("pause");
			exit(1);
		}

		if (nKeyFrames < 4 && strcmp("visibility", motion))
		{
			printf("Error: there must be at least 4 keyframes for %s.\n", name);
			system("pause");
			exit(1);
		}

		/* check whether the motion is a "transl" or a "rot" or a "scale" */
		if (strcmp(motion, "transl") == 0)
		{
			PKeys = (PositionKey*)calloc(nKeyFrames, sizeof(PositionKey));
			for (i = 0; i < nKeyFrames; i++)
			{
				ret = fscanf(fp, " %f %f %f %f %f %f %f", &time, &x, &y, &z,
					&te, &co, &bi);
				if (ret != 7)
				{
					printf("error in parsing translation keyframes for %s\n",
						animation->name);
					system("pause");
					exit(1);
				}
				PKeys[i].t = time;
				PKeys[i].P.x = x;
				PKeys[i].P.y = y;
				PKeys[i].P.z = z;
				PKeys[i].tension = te;
				PKeys[i].continuity = co;
				PKeys[i].bias = bi;
			}
			animation->translations = KB_PosInitialize(nKeyFrames, PKeys);
			free(PKeys);
		}
		else if (strcmp(motion, "rot") == 0)
		{
			RKeys = (RotationKey*)calloc(nKeyFrames, sizeof(RotationKey));
			for (i = 0; i < nKeyFrames; i++)
			{
				ret = fscanf(fp, " %f %f %f %f %f %f %f %f", &time, &x, &y, &z,
					&angle, &te, &co, &bi);
				if (ret != 8)
				{
					printf("error in parsing rotation keyframes for %s\n",
						animation->name);
					system("pause");
					exit(1);
				}
				RKeys[i].t = time;
				RKeys[i].Rot.x = x;
				RKeys[i].Rot.y = y;
				RKeys[i].Rot.z = z;
				RKeys[i].Rot.angle = angle * M_PI / 180.0;
				RKeys[i].tension = te;
				RKeys[i].continuity = co;
				RKeys[i].bias = bi;
			}
			animation->rotations = KB_RotInitialize(nKeyFrames, RKeys);
			free(RKeys);
		}
		else if (strcmp(motion, "scale") == 0)
		{
			PKeys = (PositionKey*)calloc(nKeyFrames, sizeof(PositionKey));
			for (i = 0; i < nKeyFrames; i++)
			{
				ret = fscanf(fp, " %f %f %f %f %f %f %f", &time, &x, &y, &z,
					&te, &co, &bi);
				if (ret != 7)
				{
					printf("error in parsing scale keyframes for %s\n",
						animation->name);
					system("pause");
					exit(1);
				}
				PKeys[i].t = time;
				PKeys[i].P.x = x;
				PKeys[i].P.y = y;
				PKeys[i].P.z = z;
				PKeys[i].tension = te;
				PKeys[i].continuity = co;
				PKeys[i].bias = bi;
			}
			animation->scales = KB_PosInitialize(nKeyFrames, PKeys);
			free(PKeys);
		}
		else if (strcmp(motion, "visibility") == 0)
		{
			VisKey *viskeys = (VisKey*)calloc(nKeyFrames, sizeof(VisKey));
			for (i = 0; i < nKeyFrames; i++)
			{
				ret = fscanf(fp, " %f %d", &time, &visibility);
				if (ret != 2)
				{
					printf("error in parsing visibility keyframes for %s\n",
						animation->name);
					system("pause");
					exit(1);
				}
				viskeys[i].time = time;
				viskeys[i].visibility = visibility;
			}
			animation->visibilities = viskeys;
			animation->numVisibilities += nKeyFrames;
		}
		else
		{
			printf("Error: unknown keyframe type (%s). Must be transl, rot, or scale.\n", motion);
			exit(1);
		}
		eatWhitespace(fp);
	}
}

static void parseFill(FILE *fp, World* ptr)
{
	float kd, ks, phong_pow, t, ior;
	Vec3f col;
	int moreparams;

	moreparams = getc(fp);
	if (moreparams != 'm')
	{
		ungetc(moreparams, fp);
		moreparams = 0;
	}

	if (moreparams)  /* parse the extended material description */
	{
		Vec3f amb, dif, spc;
		if (fscanf(fp, "%f %f %f", &amb[X], &amb[Y], &amb[Z]) != 3)
		{
			printf("fill material ambient syntax error");
			exit(1);
		}
		if (fscanf(fp, "%f %f %f", &dif[X], &dif[Y], &dif[Z]) != 3)
		{
			printf("fill material diffuse syntax error");
			exit(1);
		}
		if (fscanf(fp, "%f %f %f", &spc[X], &spc[Y], &spc[Z]) != 3)
		{
			printf("fill material specular syntax error");
			exit(1);
		}
		if (fscanf(fp, "%f %f %f", &phong_pow, &t, &ior) != 3)
		{
			printf("fill material (phong, transp, IOR) syntax error");
			exit(1);
		}
		/* add your extended material here
	 * e.g., viAddExtendedMaterial(amb,dif,spc,4.0*phong_pow,t,ior);
		 */
		for (int w = 0; w < 3; w++) {
			g_amb[w] = amb[w];
			g_diff[w] = dif[w];
			g_spec[w] = spc[w];
		}
		g_Shine = phong_pow;
		g_T = t;
		g_index_of_refraction = ior;
	}
	else   /* parse the old NFF description of a material */
	{
		if (fscanf(fp, "%f %f %f", &col[X], &col[Y], &col[Z]) != 3)
		{
			printf("fill color syntax error");
			exit(1);
		}

		if (fscanf(fp, "%f %f %f %f %f", &kd, &ks, &phong_pow, &t, &ior) != 5)
		{
			printf("fill material syntax error");
			exit(1);
		}

		/* add the normal NFF material here
	 * e.g., viAddMaterial(col,kd,ks,4.0*phong_pow,t,ior);
		 */

	}
}

static void getVectors(FILE *fp, char *type, int *num_vecs, Vec3f **vecs)
{
	int num, q;
	Vec3f *v = NULL;

	if (fscanf(fp, "%d", &num) != 1)
	{
		printf("Error: could not parse mesh (expected 'num_%s').\n", type);
		exit(1);
	}


	v = (Vec3f*)malloc(sizeof(Vec3f)*num);
	if (v == NULL)
	{
		printf("Error: could not allocate memory for vertices of mesh.\n");
		exit(1);
	}

	for (q = 0; q < num; q++)
	{
		if (fscanf(fp, "%f %f %f ", &v[q][X], &v[q][Y], &v[q][Z]) != 3)
		{
			printf("Error: could not read %d %s of mesh.\n", num, type);
			exit(1);
		}
	}
	*vecs = v;
	*num_vecs = num;
}

static void getTextureCoords(FILE *fp, char *texturename, int *num, Vec2f **vecs)
{
	int q;
	int num_txts;
	Vec2f *txts;
	if (fscanf(fp, "%d", &num_txts) != 1)
	{
		printf("Error: could not parse mesh (expected 'num_txts').\n");
		exit(1);
	}
	txts = (Vec2f*)malloc(sizeof(Vec2f)*num_txts);
	if (txts == NULL)
	{
		printf("Error: could not allocate memory for texturecoords of mesh.\n");
		exit(1);
	}
	fscanf(fp, "%s", texturename);
	for (q = 0; q < num_txts; q++)
	{
		if (fscanf(fp, "%f %f", &txts[q][X], &txts[q][Y]) != 2)
		{
			printf("Error: could not read %d texturecoords of mesh.\n", num_txts);
			exit(1);
		}
	}
	*num = num_txts;
	*vecs = txts;
}

static void getTriangles(FILE *fp, int *num_tris, unsigned short **indices,
	Vec3f *verts, Vec3f *norms, Vec2f *txts)
{
	int num;
	int q, w;
	int allocsize;
	unsigned short *idx;
	int i, v[3], n[3], t[3];

	allocsize = 3;
	if (norms) allocsize += 3;
	if (txts) allocsize += 3;

	if (fscanf(fp, "%d", &num) != 1)
	{
		printf("Error: could not parse mesh (expected 'num_triangles').\n");
		exit(1);
	}

	idx = (unsigned short *)malloc(num*allocsize * sizeof(unsigned short));
	if (idx == NULL)
	{
		printf("Error: could not allocate memory for indices of mesh.\n");
		exit(1);
	}
	i = 0;
	for (q = 0; q < num; q++)
	{
		if (fscanf(fp, "%d %d %d", &v[0], &v[1], &v[2]) != 3)
		{
			printf("Error: could not read %d vertex indices of mesh.\n", num);
			exit(1);
		}

		if (norms)
		{
			if (fscanf(fp, "%d %d %d", &n[0], &n[1], &n[2]) != 3)
			{
				printf("Error: could not read %d set of normal indices of mesh.\n", num);
				exit(1);
			}
		}

		if (txts)
		{
			if (fscanf(fp, "%d %d %d", &t[0], &t[1], &t[2]) != 3)
			{
				printf("Error: could not read %d texturecoord indices of mesh.\n", num);
				exit(1);
			}
		}

		/* indices appear in this order: [texture] [normals] vertices. []=optional */

		for (w = 0; w < 3; w++)
		{
			if (txts) {
				idx[i++] = t[w];
//				printf("tv: %d\n", t[w]);
			}
			if (norms)
			{
				idx[i++] = n[w];
//				printf("nv: %d\n", n[w]);
			}
			idx[i++] = v[w];
//			 printf("vv: %d\n",v[w]); 
		}
	}
	*indices = idx;
	*num_tris = num;
}

static void parseMesh(FILE *fp, World* ptr)
{
	char str[200];
	int num_verts, num_norms, num_txts, num_tris;
	Vec3f *verts = NULL, *norms = NULL;
	Vec2f *txts = NULL;
	unsigned short *indices;
	char texturename[200];
	bool use_ppm = false;

	if (fscanf(fp, "%s", str) != 1)
	{
		printf("Error: could not parse mesh (could not find 'vertices').\n");
		exit(1);
	}
	if (strcmp(str, "vertices"))
	{
		printf("Error: could not parse mesh (expected 'vertices').\n");
		exit(1);
	}
	getVectors(fp, str, &num_verts, &verts);

	fscanf(fp, "%s", str);
	if (!strcmp(str, "normals"))
	{
		getVectors(fp, str, &num_norms, &norms);
		fscanf(fp, "%s", str);
	}
	if (!strcmp(str, "texturecoords"))
	{
		getTextureCoords(fp, texturename, &num_txts, &txts);
		fscanf(fp, "%s", str);

		use_ppm = true;
	}
	if (!strcmp(str, "triangles"))
	{
		getTriangles(fp, &num_tris, &indices, verts, norms, txts);
	}
	else
	{
		printf("Error: expected 'triangles' in mesh.\n");
		exit(1);
	}
	/* add a mesh here
	 * e.g.,viAddMesh(verts,num_verts,norms,num_norms,txts,num_txts,texturename,indices,num_tris);
	 */
	Mesh* m = new Mesh;
	m->num_triangles = num_tris;
	m->num_vertices = num_verts;

//	assert(num_verts == num_txts);
	m->vertices.resize(num_verts);
	if (txts) {
		m->u.resize(num_verts);
		m->v.resize(num_verts);
	}
	for (int j = 0; j < num_verts; j++) {
		m->vertices[j] = vec3(verts[j][X],verts[j][Y],verts[j][Z]);
		if (txts) {
			m->u[j] = txts[j][0];
			m->v[j] = txts[j][1];
		}
	}

	m->vertex_faces.resize(num_tris);
	m->faceuv_indices.resize(num_tris);
	m->facenormal_indices.resize(num_tris);

	int step = 3;
	if (txts)
		step += 3;
	if (norms)
		step += 3;
	for (int j = 0; j < num_tris; j++) {

		m->faceuv_indices[j].resize(3);
		m->facenormal_indices[j].resize(3);
		m->vertex_faces[j].resize(3);
		
		int i = 0;
		for (int w = 0; w < 3; w++)
		{
			if(txts)
				m->faceuv_indices[j][w] = indices[j*step + i++];
			if(norms)
				m->facenormal_indices[j][w] = indices[j*step + i++];
				
			m->vertex_faces[j][w] = indices[j*step + i++ ];
//			printf("vv: %d\n", m->vertex_faces[j][w]);
		}
	}

	//// debug
	//for (int j = 0; j < num_tris; j++) {
	//	for (int i = 0; i < 3; i++) {
	//		std::cout << m->vertex_faces[j][i] <<" " << m->faceuv_indices[j][i]<< std::endl;
	//	}
	//}

	// set SV material.

//	m->set_mesh_material(phong_ptr);
	Texture* texture_ptr = 0;

	if (use_ppm) {
		Image* image_ptr = new Image;
		std::string full_tn = std::string(texturename);
		full_tn = prefix_dir + full_tn;
		image_ptr->read_file(full_tn.c_str());
		ImageTexture* tex_ptr = new ImageTexture;
		tex_ptr->set_image(image_ptr);
		texture_ptr = tex_ptr;
	}
	else {
		// TODO
		texture_ptr = new ConstantColor(vec3(1.0, 1.0, 1.0));
	}

	SVPhong* thisone = new SVPhong;
	shared_ptr<ConstantColor> amb_ptr(new ConstantColor(vec3(g_amb[0], g_amb[1], g_amb[2])));
	shared_ptr<ConstantColor> dif_ptr(new ConstantColor(vec3(g_amb[0], g_amb[1], g_amb[2])));
	shared_ptr<ConstantColor> spc_ptr(new ConstantColor(vec3(g_amb[0], g_amb[1], g_amb[2])));
	shared_ptr<Texture> txt_ptr(texture_ptr);
	thisone->set_ka(amb_ptr);
	thisone->set_kd(dif_ptr);
	thisone->set_ks(spc_ptr);

	thisone->set_cd(txt_ptr);
	thisone->set_exp(g_Shine);
	m->set_mesh_material(thisone);

	vector<hitable*> tar;
	for (int j = 0; j < m->num_triangles; j++) {
		int a = m->vertex_faces[j][0];
		int b = m->vertex_faces[j][1];
		int c = m->vertex_faces[j][2];

		MeshTriangle* obj_ptr = new MeshTriangle(m, a, b, c);
		tar.push_back(obj_ptr);
	}

	BVH* bvh_ptr = new BVH(tar, 0, tar.size() - 1);

	hitable* obj_ptr = 0;
	if (m->num_triangles > 20)
		obj_ptr = new BVH(tar,0,tar.size()-1);
	else
		obj_ptr = new MeshAggregate(m);

	Instance* inst_ptr = new Instance(obj_ptr);
	inst_ptr->set_material(thisone);

	if (transform_st) {
		for (int it = transform_st->translate.size()-1; it >=0; it--) {
					
			inst_ptr->scale(transform_st->scale[it]);	
			inst_ptr->rotate_axis(transform_st->degree[it], transform_st->rotate_axis[it]);
			inst_ptr->translate(transform_st->translate[it]);
		}
		/*int it = transform_st->translate.size() - 1;
		inst_ptr->scale(transform_st->scale[it]);
		inst_ptr->rotate_axis(transform_st->degree[it], transform_st->rotate_axis[it]);
		inst_ptr->translate(transform_st->translate[it]);*/
	}

	if (std::string(texturename) == "garden.ppm")
		inst_ptr->name = texturename;
	else
		inst_ptr->name = "hh";
	
	ptr->add_object(inst_ptr);
}

static void parseXform(FILE *f, World* ptr)
{
	char name[100];
	char ch;
	int is_static;

	is_static = getc(f);
	if (is_static != 's')
	{
		ungetc(is_static, f);
		is_static = 0;
	}

	if (is_static)  /* is the transform a static one ? */
	{
		Vec3f scale, trans, rot;
		float deg;

		if (fscanf(f, " %f %f %f %f %f %f %f %f %f %f", &scale[0], &scale[1], &scale[2],
			&rot[0], &rot[1], &rot[2], &deg,
			&trans[0], &trans[1], &trans[2]) != 10)
		{
			printf("Error: could not read static transform.\n");
			exit(1);
		}
		eatWhitespace(f);
		ch = getc(f);
		if (ch != '{')
		{
			printf("Error: { expected.\n");
			exit(1);
		}

		/* add a static transform here
		 * e.g.,viAddStaticXform(scale,rot,deg,trans);
		 */
		if (transform_st == 0)
			transform_st = new transform_global;

		transform_st->translate.push_back(vec3(trans[0],trans[1],trans[2]));
		transform_st->rotate_axis.push_back(vec3(rot[0], rot[1], rot[2]));
		transform_st->scale.push_back(vec3(scale[0], scale[1], scale[2]));
		transform_st->degree.push_back(deg);
		
	}
	else   /* keyframe animated transform */
	{
		if (fscanf(f, "%s", name) != 1)
		{
			printf("Error: could not read transform name.\n");
			exit(1);
		}
		eatWhitespace(f);
		ch = getc(f);
		if (ch != '{')
		{
			printf("Error: { expected.\n");
			exit(1);
		}

		/* add an animated transform here
		 * e.g., viAddXform(name);
		 */
	}
}

static void EndXform() {
	assert(transform_st->translate.size() != 0);
	transform_st->translate.pop_back();
	transform_st->rotate_axis.pop_back();
	transform_st->degree.pop_back();
	transform_st->scale.pop_back();
}

bool viParseFile(FILE* f, World* ptr) {
	int ch;

	while ((ch = getc(f)) != EOF)
	{
//		std::cout << (char)ch << std::endl;
		switch (ch)
		{
		case ' ':   /* white space */
		case '\t':
		case '\n':
		case '\f':
		case '\r':
			continue;
		case '#':   /* comment */
		case '%':   /* comment */
			parseComment(f); /* ok */
			break;
		case 'v':   /* view point */
			parseViewpoint(f, ptr); /* ok */
			break;
		case 'l':   /* light source */
			parseLight(f, ptr); /* ok */
			break;
		case 'b':   /* background color */
			parseBackground(f, ptr); /* ok */
			break;
		case 'f':   /* fill material */
			parseFill(f, ptr); /* ok */
			break;
		case 'c':   /* cylinder or cone */
	//		parseCone(f); /* ok */
			break;
		case 's':   /* sphere */
			parseSphere(f, ptr); /* ok */
			break;
		case 'p':   /* polygon or patch */
	//		parsePoly(f);
			break;
		case 'i':   /* include another file */
			parseInclude(f,ptr);  /* ok */
			break;
		case 'd':   /* detail level of file (used to exclude objects from rendering) */
			parseDetailLevel(f); /* ok */
			break;
		case 't':  /* textured triangle, or texture tripatch, or animated triangle */
	//		parseTextureStuff(f);
			break;
		case 'x':  /* transform */
			parseXform(f, ptr);
			break;
		case '}':
			EndXform();
			break;
		case 'a':  /* animation parameters */
			parseA(f,ptr);
			break;
		case 'k':  /* key frames for transform (or the camera) */
			parseKeyFrames(f,ptr);
			break;
		case 'm':  /* triangle mesh */
			parseMesh(f, ptr);
			break;
		default:    /* unknown */
			printf("unknown NFF primitive code: %c\n", ch);
//			system("pause");
			//		exit(1);
		}
	}
	return true;
}