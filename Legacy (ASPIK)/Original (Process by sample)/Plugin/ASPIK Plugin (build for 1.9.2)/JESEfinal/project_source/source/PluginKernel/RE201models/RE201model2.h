#pragma once
// Author: Jordan Evans
// Date: 16/03/2023
// 
//
//
// RE201 model 2 class developed for BCU final year university project. This model is the second of four models developed during the project. 
//
// This model is an emulation of the RE201 using a variant of the VA tone stack modelling algorithm proposed by Yeh (2006).
// Tape magnetisation is implemented as a hysteresis loop by using a modified version of Chowdhurys (2019) chowtape algorithm. 
// The class implements wow and flutter using a signal model similar to that proposed in DAFX (Zoltzer, 2012) and applys 8x oversampling using FIR filters.
// The class implements reverb using an IR of the reverb setting of a Boss RE-2 and is applied using HIFI-LOFIs Two Stage FFT Convolver object.
// The class is designed for use in mono and stereo plugins. Support for channel layouts higher than this may be implemented at a later date. 


#include "TapeDelayDynamic.h"
#include "Reverb.h"
#include "vatonestack.h"

class RE201model2 
{
	public:

	RE201model2(){}
	~RE201model2(){}

	void reset(float sampleRate, unsigned int osamount, bool solverorder) {

		SampleRate = sampleRate;
		tonestackl.reset(SampleRate);
		tonestackr.reset(SampleRate);
		tdl.reset(SampleRate, osamount, solverorder);
		tdr.reset(SampleRate, osamount, solverorder);
		verbl.reset(SampleRate);
		verbr.reset(SampleRate);

	}

	void updateParams(float low, float high, float intensity, float delaytime, int playheadenabled[], bool delayenabled, float delayamount, bool reverbenabled, float reverbamount, float inputlevel, float wetdry) {

		tonestackl.updateFilterCoefficients(low, high);
		tonestackr.updateFilterCoefficients(low, high);
		tdl.setParams(intensity, delaytime, playheadenabled);
		tdr.setParams(intensity, delaytime, playheadenabled);

		DelayEnabled = delayenabled;
		DelayAmount = delayamount;
		ReverbEnabled = reverbenabled;
		ReverbAmount = reverbamount;
		inputLevel = inputlevel;
		wetDry = wetdry;



	}

	float processSample(float xn) {

		float xnts = tonestackl.processSample(xn * inputLevel);

		float xnfilt = tdl.processSample(xnts);

		float verb = verbl.processSample(xn);

		float yn = (xnfilt * DelayAmount) + (verb * ReverbEnabled * ReverbAmount);

		return ((xnts * (1.f - wetDry)) + (yn * wetDry));

	}

	void processFrame(const float* inputs, float* outputs, bool monoin) {

		float xnl = inputs[0] * inputLevel;
		double xnr = monoin == false ? inputs[1] * inputLevel : xnl;

		float xntsl = tonestackl.processSample(xnl);
		float xntsr = tonestackr.processSample(xnr);

		float xnfiltl = tdl.processSample(xntsl);
		float xnfiltr = tdr.processSample(xntsr);

		float verbL = verbl.processSample(xnl);
		float verbR = verbr.processSample(xnr);

		float ynl = (xnfiltl * DelayEnabled * DelayAmount) + (verbL * ReverbEnabled * ReverbAmount);
		float ynr = (xnfiltr * DelayEnabled * DelayAmount) + (verbR * ReverbEnabled * ReverbAmount);

		outputs[0] = (xntsl * (1.f - wetDry)) + (ynl * wetDry);
		outputs[1] = (xntsr * (1.f - wetDry)) + (ynr * wetDry);

		return;


	}

private:

	float SampleRate = 0.0f;

	VAToneStack tonestackl, tonestackr;
	TapeDelayModelDynamic tdl, tdr;
	ReverbModelRE2 verbl, verbr;

	bool DelayEnabled = false;
	bool ReverbEnabled = false;

	float DelayAmount = 0.0f;
	float ReverbAmount = 0.0f;
	float inputLevel = 0.0f;
	float wetDry = 0.0f;

};
