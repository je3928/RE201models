#include "Reverbprocessor.h"

void ReverbProcessor::Reset(float sampleRate, int numChannels) 
{
    // Clear previous DSP objects
    Reverb.clear();

    for (int channel = 0; channel < numChannels; channel++) 
    {
        // Create reverb object for channel
        Reverb.push_back(std::make_unique<ReverbModelRE2>());

        // Reset reverb object
        Reverb[channel]->reset(sampleRate);

    }

    // Assign channel count to object for DSP
    NumChannels = numChannels;

}

void ReverbProcessor::UpdateParameters(int reverbEnabled, float reverbAmount) 
{
    // Assign plugin parameters to object for DSP
    ReverbEnabled = reverbEnabled;
    ReverbAmount = reverbAmount;
}


void ReverbProcessor::ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize)
{
   
    for (int channel = 0; channel < NumChannels; channel++)
    {
        for (int sample = 0; sample < blockSize; sample++)
        {
            // Apply DSP
            buffer[channel][sample] = ((Reverb[channel]->processSample(buffer[channel][sample])) * ReverbAmount * ReverbEnabled);

        }
    }

}
