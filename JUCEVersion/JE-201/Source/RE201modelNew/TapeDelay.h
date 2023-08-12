#pragma once

#include "dependancies/CircularBuffer.h"
#include "dependancies/wowflutter.h"
#include "dependancies/OnePole.h"
#include "dependancies/tapemag.h"
#include "dependancies/Biquad.h"
#include <vector>


class TapeDelayProcessor 
{

public:

    TapeDelayProcessor();
    ~TapeDelayProcessor();

    void Reset(float sampleRate, int numChannels); 
    void UpdateParameters(float intensity, float delaytime, int playheadEnabled[], int delayEnabled, float delayAmount);
    void ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize);


private:

    // DSP objects
    std::vector<std::unique_ptr<CircularBuffer>> PH1;
    std::vector<std::unique_ptr<CircularBuffer>> PH2;
    std::vector<std::unique_ptr<CircularBuffer>> PH3;
    std::vector<std::unique_ptr<OnePole>> BallisticsFilters;
    std::vector<std::unique_ptr<WowFlutterSignalModel>> WFProcessors;
    std::vector<std::unique_ptr<Biquad>> TSDE_LPfilters;
    std::vector<std::unique_ptr<Biquad>> TSDE_HPfilters;

    // Plugin variables
    int NumChannels = 2;
    
    float Intensity = 0.0f;
    float DelayTime = 0.0f;
    float SampleRate = 0.0f;

    int DelayEnabled = 0;
    float DelayAmount = 0.0f;

    // Playhead States
    int PH1enabled = 0;
    int PH2enabled = 0;
    int PH3enabled = 0;

    // Linear mapping function
	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}


};
