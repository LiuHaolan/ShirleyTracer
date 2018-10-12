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

vec3 Sampler::sample_hemisphere(){
	return (hemisphere_samples[count++ % (num_samples * num_sets)]);
}

void Sampler::map_samples_to_unit_disk() {

	float r, phi;
	vec2 sp;

	disk_samples.reserve(samples.size());
	for (int j = 0; j < samples.size(); j++) {
		sp[0] = 2.0*samples[j].x() - 1.0;
		sp[1] = 2.0*samples[j].y() - 1.0;

		float mult = (M_PI) / 4.0;

		if (sp.x() > sp.y()) {
			if (sp.x() > -sp.y()) {
				r = sp.x();
				phi =  mult * (sp.y()) / (sp.x());
			}
			else {
				r = -sp.y();
				phi = mult * (6.0 - sp.x() / sp.y());
			}
		}
		else {
			if (sp.x() > -sp.y()) {
				r = sp.y();
				phi = mult * (4+(sp.y()) / (sp.x()));
			}
			else {
				r = -sp.x();
				phi = mult * (6.0 - sp.x() / sp.y());
			}
		}
	}
}

void Sampler::map_samples_to_hemisphere(const float e) {
	hemisphere_samples.resize(num_samples * num_sets);
	for (int j = 0; j < samples.size(); j++) {
		float phi = 2.0*M_PI * samples[j].x();
		float cos_theta = pow(1 - samples[j].y(), 1.0 / (e + 1.0));
		float sin_theta = sqrt(1 - cos_theta * cos_theta);
		float pu = sin_theta * cos(phi);
		float pv = sin_theta * sin(phi);
		float pw = cos_theta;

		hemisphere_samples.push_back(vec3(pu, pv, pw));
	}
}