#pragma once
// Author: Jordan Evans
// Date: 16/03/2023
// 
//
//
// RE201 model 4 class developed for BCU final year university project. This model is the fourth of four models developed during the project. 
//
// This model is an emulation of the RE201 using using Wave Digital Fitler shelving filters for tone stack emulation.
// The WDF shelving filters were developed using the chowdsp_wdf library.
// Tape magnetisation is implemented as a hysteresis loop by using a modified version of Chowdhurys (2019) chowtape algorithm.  
// The class implements wow and flutter using a signal model similar to that proposed in DAFX (Zoltzer, 2012) and applys 8x oversampling using FIR filters.
// The class implements reverb using an IR of the reverb setting of a Boss RE-2 and is applied using HIFI-LOFIs Two Stage FFT Convolver object.
// The class is designed for use in mono and stereo plugins. Support for channel layouts higher than this may be implemented at a later date. 


#include "TapeDelayDynamic.h"
#include "Reverb.h"
#include "wdftonestack.h"
#include "dependancies/Biquad.h"

class RE201model4 
{
	public:

	RE201model4(){}
	~RE201model4(){}

	// Reset algorithm
	void reset(float sampleRate, unsigned int osamount, bool solverorder) {

		SampleRate = sampleRate;
		tonestackl.prepare(SampleRate);
		tonestackr.prepare(SampleRate);
		tdl.reset(SampleRate, osamount,solverorder);
		tdr.reset(SampleRate, osamount,solverorder);
		verbl.reset(SampleRate);
		verbr.reset(SampleRate);
		tsdeL.setBiquad(bq_type_lowpass, 9000.f / SampleRate, 0.5f, 0.f);
		tsdeR.setBiquad(bq_type_lowpass, 9000.f / SampleRate, 0.5f, 0.f);
		hpL.setBiquad(bq_type_highpass, 40.f / SampleRate, 0.25f, 0.f);
		hpR.setBiquad(bq_type_highpass, 40.f / SampleRate, 0.25f, 0.f);



	}

	// Update parameters
	void updateParams(float low, float high, float intensity, float delaytime, int playheadenabled[], bool delayenabled, float delayamount, bool reverbenabled, float reverbamount, float inputlevel, float wetdry) {

		tonestackl.updateParams(low, high);
		tonestackr.updateParams(low, high);
		tdl.setParams(intensity, delaytime, playheadenabled);
		tdr.setParams(intensity, delaytime, playheadenabled);

		float cutoff = map(delaytime, 200.f, 50.f, 5000.f, 15000.f);

		tsdeL.setFc(cutoff / SampleRate);
		tsdeR.setFc(cutoff / SampleRate);


		DelayEnabled = delayenabled;
		DelayAmount = delayamount;
		ReverbEnabled = reverbenabled;
		ReverbAmount = reverbamount;
		inputLevel = inputlevel;
		wetDry = wetdry;



	}

	// Process sample
	float processSample(float xn) {

		float xnts = tonestackl.processSample(xn * inputLevel);

		float xnfilt = tdl.processSample(xnts);

		xnfilt = hpL.process(tsdeL.process(xnfilt));

		float verb = verbl.processSample(xn);

		float yn = (xnfilt * DelayAmount) + (verb * ReverbEnabled * ReverbAmount);

		return ((xnts * (1.f - wetDry)) + (yn * wetDry));

	}

	// Process frame if you want stereo processing
	void processFrame(const float* inputs, float* outputs, bool monoin) {

		float xnl = inputs[0] * inputLevel;
		double xnr = monoin == false ? inputs[1] * inputLevel : xnl;

		float xntsl = tonestackl.processSample(xnl);
		float xntsr = tonestackr.processSample(xnr);

		float xnfiltl = tdl.processSample(xntsl);
		float xnfiltr = tdr.processSample(xntsr);

		xnfiltl = hpL.process(tsdeL.process(xnfiltl));
		xnfiltr = hpR.process(tsdeR.process(xnfiltr));

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

	float inputLevel = 0.0f;
	float wetDry = 0.0f;

	// Software models
	WDFToneStack tonestackl, tonestackr;
	TapeDelayModelDynamic tdl, tdr;
	ReverbModelRE2 verbl, verbr;
    Biquad tsdeL, tsdeR, hpL,hpR;

	// Delay and reverb enabled
	bool DelayEnabled = false;
	bool ReverbEnabled = false;

	// Continouous parameters
	float DelayAmount = 0.0f;
	float ReverbAmount = 0.0f;

	// Linear mapping function
	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

};