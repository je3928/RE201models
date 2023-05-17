#pragma once

#include <cmath>
#include "dependancies/OnePole.h"
#include "dependancies/CircularBuffer.h"
#include "dependancies/wowflutter.h"
#include "OversamplerStatic.h"

class TapeDelayModelStatic
{

public:

	TapeDelayModelStatic() {}
	~TapeDelayModelStatic() {}

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
	void reset(float sampleRate, int newosamount)
	{

		init(sampleRate);
		wowflutter.reset(SampleRate);
		os.reset(sampleRate, newosamount);

	}

	// Set parameters
	void setParams(float intensity, float delaytime, int playheadEnabled[])
	{
		Intensity = intensity; 
		delayTime = delaytime;

		PH1enabled = playheadEnabled[0];
		PH2enabled = playheadEnabled[1];
		PH3enabled = playheadEnabled[2];


	}

	// Process sample
	float processSample(float xn) {

		xn = os.processSample(xn);

		float delayTimeSmoothed = ballisticsfilter.process(delayTime);

		float WF = wowflutter.processSample();

		float ynPH1 = PH1.BufferRead(delayTimeSmoothed + WF, true);
		float ynPH2 = PH2.BufferRead((delayTimeSmoothed * 2) + WF, true);
		float ynPH3 = PH3.BufferRead((delayTimeSmoothed * 3) + WF, true);

		float dnPH1 = xn + Intensity * ynPH1;
		float dnPH2 = xn + Intensity * ynPH2;
		float dnPH3 = xn + Intensity * ynPH3;

		PH1.BufferWrite(dnPH1);
		PH2.BufferWrite(dnPH2);
		PH3.BufferWrite(dnPH3);


		float yn = ((ynPH1 * PH1enabled) + (ynPH2 * PH2enabled) + (ynPH3 * PH3enabled));



		return yn;

	}

private:

	float SampleRate = 0.0f;

	// Buffers, control ballistics and wow and flutter objects
	CircularBuffer PH1, PH2, PH3;
	OnePole ballisticsfilter;
	WowFlutterSignalModel wowflutter;

	OversamplerStatic os;

	int PH1enabled = 0;
	int PH2enabled = 0;
	int PH3enabled = 0;;

	float delayTime = 0.0f;
	float Intensity = 0.0f;



};