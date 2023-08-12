#pragma once

#include "ToneFiltering.h"
#include "TapeMagProcessor.h"
#include "TapeDelay.h"
#include "ReverbProcessor.h"
#include "WetDryProcessor.h"

class RE201model
{

public:

    RE201model();
    ~RE201model();

    void Reset(float sampleRate, int OSamount, int numChannels); 
    void UpdateParameters(float low, float high, float intensity, float delaytime, int playheadenabled[], int delayenabled, float delayamount, int reverbenabled, float reverbamount, float inputlevel, float wetdry);
    void ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize);

private:

    // DSP objects
    std::unique_ptr<ToneStackProcessor> ToneStack = std::make_unique<ToneStackProcessor>();
    std::unique_ptr<TapeMagProcessor> TapeMag = std::make_unique<TapeMagProcessor>();
    std::unique_ptr<TapeDelayProcessor> TapeDelay = std::make_unique<TapeDelayProcessor>();
    std::unique_ptr<ReverbProcessor> Reverb = std::make_unique<ReverbProcessor>();

    // Mixer objects
    std::unique_ptr<WetDryProcessor> ToneStackMixer = std::make_unique<WetDryProcessor>();
    std::unique_ptr<WetDryProcessor> ReverbMixer = std::make_unique<WetDryProcessor>();

    // buffers for parallel processing
    std::vector<std::vector<float>> reverbbuffer;
    std::vector<std::vector<float>> tsbuffer;

    // Plugin info
    float SampleRate = 0.0f;
    int NumChannels = 2;
    int BlockSize = 512;

    bool ReverbEnabled = false;

    // Wet dry param for blending tsbuffer
    float wetDry = 0.0f;

};
