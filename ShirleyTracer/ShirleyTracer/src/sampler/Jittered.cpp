#include "Jittered.h"



void Jittered::generate_samples() {

	if (num_samples == 1) {
		for (int p = 0; p < num_sets; p++){
			vec2 sp(0.5, 0.5);
			samples.push_back(sp);
		}

		return;
	}

	int n = (int)sqrt(num_samples);
	for(int p=0;p<num_sets;p++)
		for(int j=0;j<n;j++)
			for (int i = 0; i < n; i++) {
				vec2 sp((i + randd()) / n, (j + randd()) / n);
				samples.push_back(sp);
			}
}