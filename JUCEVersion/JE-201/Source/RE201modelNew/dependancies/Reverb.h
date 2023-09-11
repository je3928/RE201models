#pragma once

#include "FFTConvolver/TwoStageFFTConvolver.h"
#include "re2ir.h"
#include "WaveguideSpringReverb.h"


class ReverbModelRE2
{

public:

	// Reset convolver
	void reset(float sampleRate)
	{
	
		SampleRate = sampleRate;

		Convolver.reset();
		Convolver.init(32, 2048, RE2IR, 286131);

		Waveguide.Reset(sampleRate);
		

	}
	
	// Process sample
	float processSample(float xn)
	{
		float output = 0.0f;

		if (!ConvOrWaveguide)
		{
			float* xnptr = &xn;

			float convout = 0.0f;

			float* pconvout = &convout;

			Convolver.process(xnptr, pconvout, 1);

			output = convout * 0.0625;

		}

		if (ConvOrWaveguide)
		{
			output = Waveguide.ProcessSample(xn);
		}


		return output;

	}

	void SetReverbType(bool convorwaveguide)
	{
		ConvOrWaveguide = convorwaveguide;
	}

	

private:

	// FFT convolver
	fftconvolver::TwoStageFFTConvolver Convolver;

	WaveguideSpringReverbNetwork Waveguide;

	float SampleRate = 0.0f;
	float irSampleRate = 0.0f;

	// Variable to 
	bool ConvOrWaveguide = false;

};
