#pragma once
#include <vector>
#include "utility.h"

using namespace std;

class vec2 {
public:
	vec2(float x_, float y_) { e[0] = x_; e[1] = y_; }
	float x() { return e[0]; }
	float y() { return e[1]; }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; };

private:
	float e[2];
};

class Sampler {
public:
	Sampler();
	Sampler(const int num_samples);

	void set_num_sets(const int np);
	int get_num_samples(void);

	virtual void generate_samples() = 0;
//	void setup_shuffled_indices();
//	void shuffle_samples();
	vec2 sample_unit_square();

protected:
	int num_samples;
	int num_sets;
	vector<vec2> samples;
	vector<int> shuffled_indices;
	unsigned long count;
	int jump;

};