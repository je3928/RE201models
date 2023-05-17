#pragma once
// Author: Jordan Evans
// Date: 16/03/2023
// 
//
//
// RE201 model 1 class developed for BCU final year university project. This model is one of four models developed during the project. 
//
// This model is an emulation of the RE201 using a variant of the VA tone stack modelling algorithm proposed by Yeh (2006) and a static error (erf(x)) function waveshaper for nonlinearity.
// The class implements wow and flutter using a signal model similar to that proposed in DAFX (Zoltzer, 2012) and applys 8x oversampling using FIR filters.
// The class implements reverb using an IR of the reverb setting of a Boss RE-2 and is applied using HIFI-LOFIs Two Stage FFT Convolver object.
// The class is designed for use in mono and stereo plugins. Support for channel layouts higher than this may be implemented at a later date. 


#include "TapeDelayStatic.h"
#include "Reverb.h"
#include "vatonestack.h"

class RE201model1 
{
	public:

	RE201model1(){}
	~RE201model1(){}


	// Reset algorithm
	void reset(float sampleRate) {

		SampleRate = sampleRate;
		tonestackl.reset(SampleRate);
		tonestackr.reset(SampleRate);		
		tdl.reset(SampleRate);
		tdr.reset(SampleRate);
		verbl.reset(SampleRate);
		verbr.reset(SampleRate);

	}

	// Update algorithm parameters
	void updateParams(float low, float high, float intensity, float delaytime, int playheadenabled[], bool delayenabled, float delayamount, bool reverbenabled, float reverbamount, float inputlevel, bool secondorfourth) {

		tonestackl.updateFilterCoefficients(low, high);
		tonestackr.updateFilterCoefficients(low, high);
		tdl.setParams(intensity, delaytime, playheadenabled);
		tdr.setParams(intensity, delaytime, playheadenabled);

		DelayEnabled = delayenabled;
		DelayAmount = delayamount;
		ReverbEnabled = reverbenabled;
		ReverbAmount = reverbamount;
		inputLevel = inputlevel;



	}

	// Process sample
	float processSample(float xn) {

		float xnfilt = tonestackl.processSample(xn);

		xnfilt = tdl.processSample(xnfilt);

		float verb = verbl.processSample(xn);

		float yn = (xnfilt * DelayAmount) + (verb * ReverbEnabled * ReverbAmount);

		return yn;

	}

	// Process frame if you want stereo processing
	void processFrame(const float* inputs, float* outputs, bool monoin) {

		

		float xnl = inputs[0] * inputLevel;
		double xnr = monoin == false ? inputs[1] * inputLevel : xnl;

		float xnfiltl = tonestackl.processSample(xnl);
		float xnfiltr = tonestackr.processSample(xnr);

		xnfiltl = tdl.processSample(xnfiltl);
		xnfiltr = tdr.processSample(xnfiltr);

		float verbL = verbl.processSample(xnl);
		float verbR = verbr.processSample(xnr);

		float ynl = (xnfiltl * DelayEnabled * DelayAmount) + (verbL * ReverbEnabled * ReverbAmount);
		float ynr = (xnfiltr * DelayEnabled * DelayAmount) + (verbR * ReverbEnabled * ReverbAmount);

		outputs[0] = ynl;
		outputs[1] = ynr;

		return;

		
	} 

	private:

	float SampleRate = 0.0f;

	// Software model objects
	VAToneStack tonestackl, tonestackr;
	TapeDelayModelStatic tdl, tdr;
	ReverbModelRE2 verbl, verbr; 

	// Enable delay and reverb
	bool DelayEnabled = false;
	bool ReverbEnabled = false;

	// Continuous params
	float DelayAmount = 0.0f;
	float ReverbAmount = 0.0f;
	float inputLevel = 0.0f;

};
