#pragma once
#include <stdio.h>
#include "vec3.h"
#include "World.h"
#include <vector>
#include <string>


#include "lights/PointLight.h"
using namespace std;

// a global record to collect some information

static vector<string> dynamic_lights_name;
static vector<int> dynamic_lights_ind;



typedef float Vec2f[2];
typedef float Vec3f[3];
typedef float Vec4f[4];
#define X 0 
#define Y 1  
#define Z 2  
#define W 3  

#define R 0  
#define G 1  
#define B 2  
#define A 3 /* alpha */

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


	if (fscanf_s(fp, " from %f %f %f", &from[0], &from[1], &from[2]) != 3) {
		printf("Parser view syntax error");
		return;
	}


	if (fscanf_s(fp, " at %f %f %f", &at[0], &at[1], &at[2]) != 3) {
		printf("Parser view syntax error");
		return;
	}

	if (fscanf_s(fp, " up %f %f %f", &up[0], &up[1], &up[2]) != 3) {
		printf("Parser view syntax error");
		return;
	}

	if (fscanf_s(fp, " angle %f", &fov_angle) != 1) {
		printf("Parser view syntax error");
		return;
	}

	if (fscanf_s(fp, " hither %f", &hither) != 1) {
		printf("Parser view syntax error");
		return;
	}

	if (hither < 0.0001) hither = 1.0f;

	if (fscanf_s(fp, " resolution %d %d", &resx, &resy) != 2) {
		printf("Parser view syntax error");
		return;
	}

	/* init your view point here:
	 * e.g, viInitViewpoint(from, at, up, fov_angle, hither, resx, resy);
	 */
	float dist_to_focus = (from - at).length();
	float aperture = 0.0;
	ptr->camera_ptr = new Camera(from, at, up, fov_angle, float(resx) / float(resy), aperture, dist_to_focus);


	return;
	
}

static void parseLight(FILE *fp,World* ptr)
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
		fscanf_s(fp, "%s", name);
	}

	if (fscanf_s(fp, "%f %f %f ", &pos[X], &pos[Y], &pos[Z]) != 3)
	{
		printf("Light source position syntax error");
		exit(1);
	}

	/* read optional color of light */
	num = fscanf_s(fp, "%f %f %f ", &col[X], &col[Y], &col[Z]);
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

	if (fscanf_s(fp, "%f %f %f", &bgcolor[X], &bgcolor[Y], &bgcolor[Z]) != 3)
	{
		printf("background color syntax error");
	//	exit(1);
	}

	/* init you background color here
	 * e.g.  viInitBackgroundColor(bgcolor);
	 */
	ptr->background_color = bgcolor;
}

bool viParseFile(FILE* f, World* ptr);