#include "Sampler.h"
#include <algorithm>

Sampler::Sampler(void)
	: num_samples(1),
	num_sets(83),
	count(0),
	jump(0) {
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}

Sampler::Sampler(const int num_sample) {
	num_samples = num_sample;
	num_sets = 83;
	count = 0;
	jump = 0;
	setup_shuffled_indices();
}

void Sampler::set_num_sets(const int np) {
	num_sets = np;
}

int Sampler::get_num_samples(void) {
	return (num_samples);
}

//vec2 Sampler::sample_unit_square() {
//	return (samples[count++ % (num_samples * num_sets)]);
//}

vec3 Sampler::sample_hemisphere(){
	if (count % num_samples == 0)
		jump = ((int)(randd()*num_sets))*num_samples;
	return hemisphere_samples[jump + shuffled_indices[jump + count++ % num_samples]];
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

void Sampler::map_samples_to_hemisphere() {
	hemisphere_samples.resize(num_samples * num_sets);
	for (int j = 0; j < samples.size(); j++) {
	/*	float phi = 2.0*M_PI * samples[j].x();
		float cos_theta = sqrt(samples[j].y());
		float sin_theta = sqrt(1 - cos_theta * cos_theta);*/
		float phi = 2.0*M_PI * randd();
		float cos_theta = sqrt(randd());
		float sin_theta = sqrt(1 - cos_theta * cos_theta);
		float pu = cos_theta * cos(phi);
		float pv = cos_theta * sin(phi);
		float pw = sin_theta;

		hemisphere_samples[j] = (vec3(pu, pv, pw));
	}
}

void Sampler::map_samples_to_hemisphere(const float e) {
	hemisphere_samples.resize(num_samples * num_sets);
	for (int j = 0; j < samples.size(); j++) {
		float phi = 2.0*M_PI * samples[j].x();
		float cos_theta = pow((1 - samples[j].y()), 1.0 / (e + 1.0));
		float sin_theta = sqrt(1 - cos_theta * cos_theta);
		float pu = sin_theta * cos(phi);
		float pv = sin_theta * sin(phi);
		float pw = cos_theta;

		hemisphere_samples[j] = (vec3(pu, pv, pw));
	}
}

void Sampler::setup_shuffled_indices() {
	shuffled_indices.reserve(num_samples*num_sets);
	vector<int> indices;

	for (int j = 0; j < num_samples; j++)
		indices.push_back(j);

	for (int p = 0; p < num_sets; p++) {
		random_shuffle(indices.begin(), indices.end());
		for (int j = 0; j < num_samples; j++)
			shuffled_indices.push_back(indices[j]);
	}
}

vec2 Sampler::sample_unit_square() {
	if (count % num_samples == 0)
		jump = ((int)(randd()*num_sets))*num_samples;
	return samples[jump + shuffled_indices[jump + count++ % num_samples]];
}