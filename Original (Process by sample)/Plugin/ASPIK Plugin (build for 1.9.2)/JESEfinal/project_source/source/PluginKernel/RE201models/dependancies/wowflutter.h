// wowflutter.h
//
// Author: Jordan Evans
// Date: 18/02/2023
//
// This file is a wow and flutter control signal generator. It is signal model which is a sum of white noise, sinusoids and the control signal being modulated.

#pragma once


#include <cmath>
#include "FirFilter.h"



class SineOsc 
{

public:

	SineOsc(void) {}
	~SineOsc(void) {}

	void reset(float sampleRate) {
	
		SampleRate = sampleRate;
		ts = 1.f / SampleRate;
		dt = 0;

	}

	void setParams(float amplitude, float frequency) {
	
		Frequency = frequency;
		Amplitude = amplitude;
	
	}

	float returnCurrentValue() {

		dt += 1;

		dt %= (int)(SampleRate); 

	
		return Amplitude * sin(2.f * M_PI * Frequency * ((float)dt * ts)); 
	
	}


private:


	float SampleRate = 0.0f;
	unsigned int dt = 0;
	float ts = 0.0f;
	float Frequency = 0.0f;
	float Amplitude = 0.0f;

};




class WhiteNoiseOsc
{

public:

	WhiteNoiseOsc(void) {}
	~WhiteNoiseOsc(void) {}


	void setParams(float amplitude) {
		
		Amplitude = amplitude;

	}

	float returnCurrentValue() {

		return Amplitude * doWhiteNoise();

	}


private:


	float Amplitude = 0.0f;

	float doWhiteNoise()
	{
		float noise = 0.0;

#if defined _WINDOWS || defined _WINDLL
		// fNoise is 0 -> 32767.0
		noise = (float)rand();

		// normalize and make bipolar
		noise = 2.f * (noise / 32767.f) - 1.f;
#else
		// fNoise is 0 -> ARC4RANDOMMAX
		noise = (float)arc4random();

		// normalize and make bipolar
		noise = 2.0 * (noise / ARC4RANDOMMAX) - 1.0;
#endif

		return noise;

	}


};

class WowFlutterSignalModel
{

public: 

	WowFlutterSignalModel(){}
	~WowFlutterSignalModel() {}

	void init() {
	
		one.setParams(1.f, 1.f);
		two.setParams(0.5, 4.f);
		three.setParams(0.2f, 12.f);
		noise.setParams(8.f);
	
		
	
	}

	void reset(float sampleRate) {

		one.reset(sampleRate);
		two.reset(sampleRate);
		three.reset(sampleRate);
		filt.setCutoffFrequency(1.f, sampleRate);

		

	}


	float processSample() {


		float sinsum = (one.returnCurrentValue() + two.returnCurrentValue() + three.returnCurrentValue() + filt.filter(noise.returnCurrentValue()));

		float yn = sinsum * 0.35;

		return yn;

	}






private:

	SineOsc one, two, three;

	WhiteNoiseOsc noise;

	FirFilter filt{ 150 };

};