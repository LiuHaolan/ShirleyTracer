#include "MultiJittered.h"

void MultiJittered::generate_samples() {
	int n = (int)sqrt((float)num_samples);
	float subcell_width = 1.0 / ((float)num_samples);

	vec2 fill_point;
	samples.resize(num_samples* num_sets);

	for(int p=0;p<num_sets;p++)
		for(int i=0;i<n;i++)
			for (int j = 0; j < n; j++) {
				samples[i*n + j + p * num_samples][0] = (j*n + i)*subcell_width + randd() * subcell_width;
				samples[i*n + j + p * num_samples][1] = (i*n + j)*subcell_width + randd() * subcell_width;
			}

	// shuffle
	for(int p=0;p<num_sets;p++)
		for(int i=0;i<num_samples;i++){
				int k = (int)(randd()*(num_samples-1));
				float t = samples[i +  p * num_samples].x();
				samples[i+ p * num_samples][0] = samples[k + p * num_samples][0];
				samples[k + p * num_samples][0] = t;
			}


	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < num_samples; i++) {
				int k = (int)(randd()*(num_samples-1));
				float t = samples[i + p * num_samples].y();
				samples[i + p * num_samples][1] = samples[k + p * num_samples][1];
				samples[k + p * num_samples][1] = t;
			}
}