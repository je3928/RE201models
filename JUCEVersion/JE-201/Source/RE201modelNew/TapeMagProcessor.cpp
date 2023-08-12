#include "TapeMagProcessor.h"

TapeMagProcessor::TapeMagProcessor()
{

}

TapeMagProcessor::~TapeMagProcessor()
{

}

// Currently only setup for OS amount of 2x, do not use for amounts higher than this yet...
void TapeMagProcessor::Reset(float sampleRate, int numChannels, int osamount)
{
    
    // Save OS amount to object for DSP processing
    OSamount = osamount;
    
    // Clear previous DSP objects
    TapeODESolvers.clear();
    InterpolationFilters.clear();
    DecimationFilters.clear();

    // Assign plugin info to object
    NumChannels = numChannels;
    SampleRate = sampleRate;

    int intTaps = 50;
    int decTaps = 100;



    for (int channel = 0; channel < numChannels; channel++)
    {

        float overSampleRate = sampleRate * osamount;

        // Initialise DSP objects for number of channels
        TapeODESolvers.push_back(std::make_unique<rk4thOrderODETapeMag>());

        // Reset tape mag solver object with oversampled rate
        TapeODESolvers[channel]->init(overSampleRate);


        // Only initialise new oversampling filters when oversampling
        if (OSamount > 1)
        {
            InterpolationFilters.push_back(std::make_unique<FirFilter>(intTaps));
            DecimationFilters.push_back(std::make_unique<FirFilter>(decTaps));

            InterpolationFilters[channel]->setCutoffFrequency(20000.f, overSampleRate);
            DecimationFilters[channel]->setCutoffFrequency(20000.f, overSampleRate);
        }

    }

    

}

void TapeMagProcessor::ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize)
{
    // Set channels to buffer size if inconsistent,
    // avoids segmentation errors when mono only.
    if (buffer.size() != NumChannels)
        NumChannels = buffer.size();
    
    // If oversampling enabled apply oversampling
    if (OSamount > 1)
    {
        // Calculate OS block size
        int OSblocksize = blockSize * OSamount;

        // Initialise OS buffer
        std::vector<std::vector<float>> OSbuffer;

        // Resize OS buffer
        OSbuffer.resize(NumChannels);
        
        // Resize OS buffer
        for (int i = 0; i < NumChannels; ++i)
        {
            OSbuffer[i].resize(OSblocksize);
        }

        // Apply oversampling
        for (int channel = 0; channel < NumChannels; channel++)
        {
            // Apply channel to OSbuffer
            for (int sample = 0; sample < blockSize; sample++)
                for (int oversampleindx = 0; oversampleindx < OSamount; oversampleindx++)
                    OSbuffer[channel][(sample * OSamount) + oversampleindx] = buffer[channel][sample];

            // Apply AA filtering and tape magnetisation
            for (int sample = 0; sample < OSblocksize; sample++)
                OSbuffer[channel][sample] = DecimationFilters[channel]->filter(TapeODESolvers[channel]->process4th(InterpolationFilters[channel]->filter(OSbuffer[channel][sample]), 15.f, 30.f));

            // Downsample
            for (int sample = 0; sample < blockSize; sample++)
                buffer[channel][sample] = OSbuffer[channel][sample * OSamount];

        }
    }
    // else apply tape magnetisation without oversampling
    else if (OSamount == 1)
    {
        for (int channel = 0; channel < NumChannels; channel++)
        {
            // Apply tape magnetisation
            for (int sample = 0; sample < blockSize; sample++)
                buffer[channel][sample] = TapeODESolvers[channel]->process4th(buffer[channel][sample], 15.f, 30.f);

          
        }
    }

}




