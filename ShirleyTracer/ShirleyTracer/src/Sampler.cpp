#include "Sampler.h"
#include <algorithm>

Sampler::Sampler(void)
	: num_samples(1),
	num_sets(83),
	count(0),
	jump(0) {
	samples.reserve(num_samples * num_sets);
	//setup_shuffled_indices();
}

Sampler::Sampler(const int num_sample) {
	num_samples = num_sample;
	num_sets = 83;
	count = 0;
	jump = 0;
}

void Sampler::set_num_sets(const int np) {
	num_sets = np;
}

int Sampler::get_num_samples(void) {
	return (num_samples);
}

vec2 Sampler::sample_unit_square() {
	return (samples[count++ % (num_samples * num_sets)]);
}