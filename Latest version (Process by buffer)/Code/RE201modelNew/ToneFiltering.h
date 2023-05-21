#pragma once

#include "dependancies/wdftonestack.h"
#include "dependancies/vatonestack.h"
#include <memory>

class ToneStackProcessor 
{

public:

    ToneStackProcessor() {};
    ~ToneStackProcessor() {};

    // Resets the tone stack processor with sample rate and number of channels.
    // set bool to 0 for VA tone filtering and 1 for WDF filtering.
    void Reset(float sampleRate, int numChannels, bool vaorwdf); 
    void UpdateParameters(float Bass, float Treble, float inputlevel);
    void ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize);

private:

    // DSP filters
    std::vector<std::unique_ptr<VAToneStack>> VAfilters;
    std::vector<std::unique_ptr<WDFToneStack>> WDFfilters;

    // False for VA model, true for WDF model. Assigned at reset.
    bool VAorWDF = false;

    // Object variables
    int NumChannels = 2;
    float InputLevel = 0.0f;
    
    // Linear mapping function
    float map(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

};
