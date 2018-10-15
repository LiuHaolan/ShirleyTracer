#pragma once
#include "..\Sampler.h"

class MultiJittered : public Sampler {
public:
	MultiJittered(const int num_sample) :Sampler(num_sample) {

		generate_samples();
	}

private:
	virtual void generate_samples();
};