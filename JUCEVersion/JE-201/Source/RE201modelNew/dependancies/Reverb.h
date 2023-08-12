#pragma once

#include "FFTConvolver/TwoStageFFTConvolver.h"
#include "re2ir.h"


class ReverbModelRE2
{

public:

	// Reset convolver
	void reset(float sampleRate)
	{
	
		SampleRate = sampleRate;

		Convolver.reset();
		Convolver.init(32, 2048, RE2IR, 286131);
		

	}
	
	// Process sample
	float processSample(float xn)
	{

		float* xnptr = &xn;

		float convout = 0.0f;

		float* pconvout = &convout;

		Convolver.process(xnptr, pconvout, 1);


		return convout * 0.0625;

	}

	

private:

	// FFT convolver
	fftconvolver::TwoStageFFTConvolver Convolver;

	float SampleRate = 0.0f;
	float irSampleRate = 0.0f;

};
