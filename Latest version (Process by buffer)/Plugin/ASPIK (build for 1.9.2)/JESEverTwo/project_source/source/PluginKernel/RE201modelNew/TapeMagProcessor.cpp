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
    // Clear previous DSP objects
    TapeODESolvers.clear();
    InterpolationFilters.clear();
    DecimationFilters.clear();

    // Assign plugin info to object
    NumChannels = numChannels;
    SampleRate = sampleRate;

    int intTaps = 50;
    int decTaps = 50;



    for (int channel = 0; channel < numChannels; channel++)
    {


        // Initialise DSP objects for number of channels
        TapeODESolvers.push_back(std::make_unique<rk4thOrderODETapeMag>());
        InterpolationFilters.push_back(std::make_unique<FirFilter>(intTaps));
        DecimationFilters.push_back(std::make_unique<FirFilter>(decTaps));

        float overSampleRate = sampleRate * osamount;

        // Reset tape mag solver object with oversampled rate
        TapeODESolvers[channel]->init(overSampleRate);
        InterpolationFilters[channel]->setCutoffFrequency(20000.f, overSampleRate);
        DecimationFilters[channel]->setCutoffFrequency(20000.f, overSampleRate);

    }

    // Save OS amount to object for DSP processing
    OSamount = osamount;


}

void TapeMagProcessor::ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize)
{
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

        // 
        for (int channel = 0; channel < NumChannels; channel++)
        {

            for (int sample = 0; sample < blockSize; sample++)
                for (int oversampleindx = 0; oversampleindx < OSamount; oversampleindx++)
                    OSbuffer[channel][(sample * OSamount) + oversampleindx] = buffer[channel][sample];

            for (int sample = 0; sample < OSblocksize; sample++)
                OSbuffer[channel][sample] = DecimationFilters[channel]->filter(TapeODESolvers[channel]->process4th(InterpolationFilters[channel]->filter(OSbuffer[channel][sample]), 15.f, 30.f));

            for (int sample = 0; sample < blockSize; sample++)
                buffer[channel][sample] = OSbuffer[channel][sample * OSamount];

        }
    } 
    // else apply tape magnetisation without oversampling
    else if (OSamount == 1)
    {
        for (int channel = 0; channel < NumChannels; channel++)
        {

            for (int sample = 0; sample < blockSize; sample++)
                buffer[channel][sample] = TapeODESolvers[channel]->process4th(buffer[channel][sample], 15.f, 30.f);

          
        }
    }

}




