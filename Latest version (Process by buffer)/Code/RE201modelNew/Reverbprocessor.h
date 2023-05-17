#pragma once

#include "dependancies/Reverb.h"
#include <memory>

class ReverbProcessor 
{

public:

    ReverbProcessor() {};
    ~ReverbProcessor() {};

    void Reset(float sampleRate, int numChannels); 
    void UpdateParameters(int reverbEnabled, float reverbAmount);
    void ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize);

private:

    // DSP objects
    std::vector<std::unique_ptr<ReverbModelRE2>> Reverb;

    // Plugin variables
    int NumChannels = 2;
    int ReverbEnabled = 0;
    float ReverbAmount = 0.0f;

};