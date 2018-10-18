#pragma once


#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <iterator>
#include "extern\tinyply.h"
using namespace tinyply;

class manual_timer
{
	std::chrono::high_resolution_clock::time_point t0;
	double timestamp{ 0.f };
public:
	void start() { t0 = std::chrono::high_resolution_clock::now(); }
	void stop() { timestamp = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - t0).count() * 1000; }
	const double & get() { return timestamp; }
};

struct float2 { float x, y; };
struct float3 { float x, y, z; };
struct double3 { double x, y, z; };
struct uint3 { uint32_t x, y, z; };
struct uint4 { uint32_t x, y, z, w; };

struct geometry
{
	std::vector<float3> vertices;
	std::vector<float3> normals;
	std::vector<float2> texcoords;
	std::vector<uint3> triangles;
};

void read_ply_file(const std::string & filepath, std::vector<float3>& verts, std::vector<uint3>& fs);

