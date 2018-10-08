#pragma once
#include "..\Sampler.h"

class Jittered : public Sampler {
public:
	Jittered(const int num_sample):Sampler(num_sample) {
		
		generate_samples();
	}

private:
	virtual void generate_samples();
};