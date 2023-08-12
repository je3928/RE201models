#include "ToneFiltering.h"

void ToneStackProcessor::Reset(float sampleRate, int numChannels, bool vaorwdf) 
{
    // Clear previous filters
    WDFfilters.clear();
    VAfilters.clear();
    
    // Check if we are using VA or WDF tone stack model and initialise correct filters.
    if(!vaorwdf)
    {
        for (int channel = 0; channel < numChannels; channel++) 
        {
            VAfilters.push_back(std::make_unique<VAToneStack>());
            VAfilters[channel]->reset(sampleRate);
        }
    } 
    else if(vaorwdf)
    {
        for (int channel = 0; channel < numChannels; channel++) 
        {
            WDFfilters.push_back(std::make_unique<WDFToneStack>());
            WDFfilters[channel]->prepare(sampleRate);
        }
    } 

    // Assign variables to object for DSP processing
    NumChannels = numChannels;
    VAorWDF = vaorwdf;

}

void ToneStackProcessor::UpdateParameters(float Bass, float Treble, float inputlevel)
{
    // Check whether we are using VA or WDF model and calculate appropriate filters.
    if (!VAorWDF)
    {
        Bass = map(Bass, 0.f, 1.f, 0.001, 0.999);
        Treble = map(Treble, 0.f, 1.f, 0.001, 0.999);
        for (int channel = 0; channel < NumChannels; channel++) 
        {
            VAfilters[channel]->updateFilterCoefficients(Bass, Treble);
        }
    }
    else if (VAorWDF)
    {
        Bass = map(Bass, 0.f, 1.f, 0.025, 4.f);
        Treble = map(Treble, 0.f, 1.f, 0.025, 4.f);
        for (int channel = 0; channel < NumChannels; channel++) 
        {
            WDFfilters[channel]->updateParams(Bass, Treble);
        }
    }

    // Assign input level to object for DSP processing
    InputLevel = inputlevel;
    

}

void ToneStackProcessor::ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize)
{
    
    // Set channels to buffer size if inconsistent,
    // avoids segmentation errors when mono only.
    if (buffer.size() != NumChannels)
        NumChannels = buffer.size();
    
    // Check if we are using VA or WDF tone stack model and apply correct DSP
   if (!VAorWDF)
    {
        for (int channel = 0; channel < NumChannels; channel++)
        {
            for (int sample = 0; sample < blockSize; sample++)
            {
                
                buffer[channel][sample] = VAfilters[channel]->processSample((buffer[channel][sample]) * InputLevel);
            }
        }
    }
    else if (VAorWDF)
    {
        for (int channel = 0; channel < NumChannels; channel++)
        {
            for (int sample = 0; sample < blockSize; sample++)
            {
                buffer[channel][sample] = WDFfilters[channel]->processSample((buffer[channel][sample]) * InputLevel);
            }
        }
    }

}



