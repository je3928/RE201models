#pragma once

#include "dependancies/CircularBuffer.h"
#include "dependancies/OnePole.h"
#include "dependancies/wowflutter.h"
#include "OversamplerDynamic.h"
#include <cmath>

#define M_PI 3.1415926535897932384626433



class TapeDelayModelDynamic
{




public:

	TapeDelayModelDynamic() {}
	~TapeDelayModelDynamic() {}
	
	// Initialise tape delay
	void init(float sampleRate)
	{
	
		SampleRate = sampleRate;

		PH1.Init(SampleRate, 700.f);
		PH2.Init(SampleRate, 700.f);
		PH3.Init(SampleRate, 700.f);

		ballisticsfilter.reset(SampleRate);
		ballisticsfilter.setAlpha(0.500);
		
		wowflutter.init();

	}

	// Reset tape delay
	void reset(float sampleRate, unsigned int osamount, bool solverorder)
	{

		init(sampleRate);
		wowflutter.reset(SampleRate);
		osdrive.reset(SampleRate, osamount, solverorder);

	}

	// Set delay params
	void setParams(float intensity, float delaytime, int playheadEnabled[])
	{

		delayTime = delaytime;

		PH1enabled = playheadEnabled[0];
		PH2enabled = playheadEnabled[1];
		PH3enabled = playheadEnabled[2];

	
		Intensity = intensity;

	}

	// Process sample
	float processSample(float xn) {


		xn = osdrive.processSample(xn);

		float delayTimeSmoothed = ballisticsfilter.process(delayTime);
		float WF = wowflutter.processSample();

		float ynPH1 = PH1.BufferRead(delayTimeSmoothed + WF, true);
		float ynPH2 = PH2.BufferRead(((delayTimeSmoothed * 2) + WF), true);
		float ynPH3 = PH3.BufferRead(((delayTimeSmoothed * 3) + WF), true);

		float dnPH1 = xn + Intensity * ynPH1;
		float dnPH2 = xn + Intensity * ynPH2;
		float dnPH3 = xn + Intensity * ynPH3;


		PH1.BufferWrite(dnPH1);
		PH2.BufferWrite(dnPH2);
		PH3.BufferWrite(dnPH3);


		float yn = ((ynPH1 * PH1enabled) + (ynPH2 * PH2enabled) + (ynPH3 * PH3enabled));



		return yn;

	}

	/*
		---- Method for applying delay with one circular buffer, currently experimental and has delay timing issues


	float processSample(float xn) {


		xn = osdrive.processSample(xn);

		float delayTimeSmoothed = ballisticsfilter.process(delayTime);
		float WF = wowflutter.processSample();

		float ynPH1 = tapebuffer.BufferRead(delayTimeSmoothed + WF, true);
		float ynPH2 = tapebuffer.BufferRead(((delayTimeSmoothed * 2) + WF), true);
		float ynPH3 = tapebuffer.BufferRead(((delayTimeSmoothed * 3) + WF), true);

		float intensitysum = (Intensity * PH1enabled) + (Intensity * PH2enabled) + (Intensity * PH3enabled);
		float intensitynormalised = Intensity / (PH1enabled + PH2enabled + PH3enabled);

		float delayedSum = (ynPH1 * PH1enabled) + (ynPH2 * PH2enabled) + (ynPH3 * PH3enabled);
		float dn = xn + intensitynormalised * delayedSum;

		tapebuffer.BufferWrite(dn);

		float yn = delayedSum;


		return yn;

	} */

	

private:

	float SampleRate = 0.0f;

	// Buffers
	CircularBuffer PH1, PH2, PH3; 

	// Control ballistics and wow and flutter
	OnePole ballisticsfilter;
	WowFlutterSignalModel wowflutter;

	
	int PH1enabled = 0; 
	int PH2enabled = 0;
	int PH3enabled = 0;


	float delayTime = 0.0f;

	float Intensity = 0.0f;

	OversamplerDynamic osdrive;
	
	

};