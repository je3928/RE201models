#include "TapeDelay.h"

TapeDelayProcessor::TapeDelayProcessor()
{
   
}

TapeDelayProcessor::~TapeDelayProcessor()
{
    // Nothing to clean up
}

void TapeDelayProcessor::Reset(float sampleRate, int numChannels) 
{

    // Clear previous DSP objects
    PH1.clear();
    PH2.clear();
    PH3.clear();
    BallisticsFilters.clear();
    WFProcessors.clear();
    TSDE_HPfilters.clear();
    TSDE_LPfilters.clear();

    // Assign plugin variables to object
    NumChannels = numChannels;
    SampleRate = sampleRate;


    for (int channel = 0; channel < numChannels; channel++) 
    {
        // Initialise DSP objects
        PH1.push_back(std::make_unique<CircularBuffer>());
        PH2.push_back(std::make_unique<CircularBuffer>());
        PH3.push_back(std::make_unique<CircularBuffer>());
        BallisticsFilters.push_back(std::make_unique<OnePole>());
        WFProcessors.push_back(std::make_unique<WowFlutterSignalModel>());
        TSDE_HPfilters.push_back(std::make_unique<Biquad>());
        TSDE_LPfilters.push_back(std::make_unique<Biquad>());

        // Reset circular buffers
        // Maximum tape buffer delay time = 700ms as maximum delay of RE201 is 600ms.
        PH1[channel]->Init(sampleRate, 700.f);
        PH2[channel]->Init(sampleRate, 700.f);
        PH3[channel]->Init(sampleRate, 700.f);

        // Reset control ballistics filter
        BallisticsFilters[channel]->reset(sampleRate);

        // Set cutoff of ballistics filter with time constant
        // 500 ms control ballistics smoothing time
        BallisticsFilters[channel]->setAlpha(0.5f);

        // Reset wow and flutter object
        WFProcessors[channel]->init();
        WFProcessors[channel]->reset(sampleRate);

        // Reset TSDE biquad filters
        TSDE_HPfilters[channel]->Init(HPF, 40.f, sampleRate, 0.25f, 0.0f);
        TSDE_HPfilters[channel]->Reset(sampleRate);
        TSDE_LPfilters[channel]->Init(LPF, 9000.f, sampleRate, 0.5f, 0.f);
        TSDE_LPfilters[channel]->Reset(sampleRate);
        
    }
    
}

void TapeDelayProcessor::UpdateParameters(float intensity, float delaytime, int playheadEnabled[], int delayEnabled, float delayAmount)
{

    // Save plugin parameters to object
    Intensity = intensity;
    DelayTime = map(delaytime, 0.f, 1.f, 200.f, 50.f);
    DelayEnabled = delayEnabled;
    DelayAmount = delayAmount;

    // Store playhead states
    PH1enabled = playheadEnabled[0];
    PH2enabled = playheadEnabled[1];
    PH3enabled = playheadEnabled[2];

    // Calculate cutoff for low pass biquad based on delay time (tape speed)
    float cutoff = map(DelayTime, 200.f, 50.f, 5000.f, 15000.f);

    // Apply variable cutoff to filters
    for(int channel = 0; channel < NumChannels; channel++)
    {
        TSDE_LPfilters[channel]->SetFc(cutoff);
    }

}

void TapeDelayProcessor::ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize)
{
    if (buffer.size() != NumChannels)
        NumChannels = buffer.size();

    for (int channel = 0; channel < NumChannels; channel++)
    {
        for (int sample = 0; sample < blockSize; sample++)
        {

            // Get current sample
            float currentSample = buffer[channel][sample];
            float xn = currentSample;

            // Get wow and flutter value
            float WF = WFProcessors[channel]->processSample();

            // Calculate smoothed delay time from control ballistics filter
		    float delayTimeSmoothed = BallisticsFilters[channel]->process(DelayTime);

            // Get playhead outputs from buffers
		    float ynPH1 = PH1[channel]->BufferRead(delayTimeSmoothed + WF, true);
		    float ynPH2 = PH2[channel]->BufferRead(((delayTimeSmoothed * 2) + WF), true);
		    float ynPH3 = PH3[channel]->BufferRead(((delayTimeSmoothed * 3) + WF), true);
            
            // Calculate each IIR comb filter
            float dnPH1 = xn + Intensity * ynPH1;
            float dnPH2 = xn + Intensity * ynPH2;
            float dnPH3 = xn + Intensity * ynPH3;

            // Write to buffers
            PH1[channel]->BufferWrite(dnPH1);
            PH2[channel]->BufferWrite(dnPH2);
            PH3[channel]->BufferWrite(dnPH3);

            // Get output based on which playheads are enabled
            float yn = ((ynPH1 * PH1enabled) + (ynPH2 * PH2enabled) + (ynPH3 * PH3enabled));

            // Apply tape speed dependant filters
            yn = TSDE_LPfilters[channel]->ProcessSample(TSDE_HPfilters[channel]->ProcessSample(yn));

            // Apply to buffer, set volume with plugin variables
            buffer[channel][sample] = (yn * DelayEnabled * DelayAmount);

        }
    }


}





