#pragma once

#include "dependancies/FFTConvolver/TwoStageFFTConvolver.h"
#include "re2ir.h"


class ReverbModelRE2
{

public:

	// Reset convolver
	void reset(float sampleRate)
	{
	
		SampleRate = sampleRate;

		Convolver.reset();
		Convolver.init(32, 512, RE2IR, 286131);
		




	}
	
	// Process sample
	float processSample(float xn)
	{
	
		xn = xn / 2.f;

		float* xnptr = &xn;

		float convout = 0.0f;

		float* pconvout = &convout;

		Convolver.process(xnptr, pconvout, 1);


		return convout / 10.f;

	}

	

private:

	// FFT convolver
	fftconvolver::TwoStageFFTConvolver Convolver;

	float SampleRate = 0.0f;
	float irSampleRate = 0.0f;

};