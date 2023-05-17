#include "RE201model.h"
#include "WetDryProcessor.h"

RE201model::RE201model()
{

}

RE201model::~RE201model()
{

}

void RE201model::Reset(float sampleRate, int OSamount) 
{
        // Initialise DSP objects
        ToneStack.reset(new ToneStackProcessor());
        TapeMag.reset(new TapeMagProcessor());
        TapeDelay.reset(new TapeDelayProcessor());
        Reverb.reset(new ReverbProcessor());

        // Reset DSP objects
        ToneStack->Reset(sampleRate, NumChannels, true);
        TapeMag->Reset(sampleRate, NumChannels, OSamount);
        TapeDelay->Reset(sampleRate, NumChannels);
        Reverb->Reset(sampleRate, NumChannels);

     
    
    SampleRate = sampleRate;

}

void RE201model::UpdateParameters(float low, float high, float intensity, float delaytime, int playheadenabled[], int delayenabled, float delayamount, int reverbenabled, float reverbamount, float inputlevel, float wetdry) 
{
       // Update DSP object parameters
       ToneStack->UpdateParameters(low, high, inputlevel);
       TapeDelay->UpdateParameters(intensity, delaytime, playheadenabled, delayenabled, delayamount);
       Reverb->UpdateParameters(reverbenabled, reverbamount);

       wetDry = wetdry;

}


void RE201model::ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize, int numChannels)
{

    // Save channels and block size
    NumChannels = numChannels;
    BlockSize = blockSize;

    // Save copy of dry signal for reverb processing
    reverbbuffer = buffer;

    // Apply tone stack processing
    ToneStack->ProcessBuffer(buffer, blockSize);

    // Save copy of tone stack filtered signal for dry wet processing 
    tsbuffer = buffer;

    // Apply tape magnetisation and delay
    TapeMag->ProcessBuffer(buffer, blockSize);
    TapeDelay->ProcessBuffer(buffer, blockSize);

    // Mix tone stack buffer with tape delay buffer
    // Scaled by 2 for appropriate levelling at ReverbMixer
    ToneStackMixer->ProcessBuffer(tsbuffer, buffer, blockSize, wetDry, 2.f);

    // Apply reverb to dry buffer
    Reverb->ProcessBuffer(reverbbuffer, blockSize);

    // Mix output of tape with output of reverb.
    // 0.5 = wet dry as internal volume variables should handle mix of these signals.
    ReverbMixer->ProcessBuffer(reverbbuffer, buffer, blockSize, 0.5f, 1.f);


}
