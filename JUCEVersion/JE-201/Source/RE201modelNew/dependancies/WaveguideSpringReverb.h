#pragma once

#include "CircularBuffer.h"
#include "AllPassFilter.h"
#include "Biquad.h"
#include <random>


class WaveguideUnit 
{
public:

    WaveguideUnit();
    ~WaveguideUnit();

    float ProcessSample(float xn);
    void Reset(float sampleRate, float HPF_FC, float LPF_FC);
    void SetParameters(float RT60, float delayTimeSeconds);



private:

    float sampleRate = 0.0f;
    float rt60 = 0.0f;
    float delayTimeSeconds = 0.0f;
    float attenuation = 0.99999f;

    CircularBuffer delayBuffer;
    AllPassFilter dispertion1, dispertion2, dispertion3, dispertion4, dispertion5;
    Biquad OutputFilter1, OutputFilter2;




};

class WaveguideSpringReverb
{

public:

    WaveguideSpringReverb() {};
    ~WaveguideSpringReverb() {};

    float ProcessSample(float xn) 
    {
        junctionleft = (reflectionright * 0.5) + xn + (reflectionleft2 * 0.5);
        junctionright = (reflectionleft * 0.5) + xn + (reflectionright2 * 0.5);

        reflectionleft = WaveguideLeft1.ProcessSample(junctionleft);
        reflectionright = WaveguideRight1.ProcessSample(junctionright);

        reflectionright2 = WaveguideRight2.ProcessSample(reflectionleft);
        reflectionleft2 = WaveguideRight2.ProcessSample(reflectionright);



        yn = reflectionleft2; 

        return yn;
    
    };

    void Reset(float sampleRate, float minDelaySeconds, float maxDelaySeconds, float HPF_FC, float LPF_FC)
    {
        WaveguideLeft1.Reset(sampleRate, HPF_FC, LPF_FC);
        WaveguideRight1.Reset(sampleRate, HPF_FC, LPF_FC);
        WaveguideLeft2.Reset(sampleRate, HPF_FC, LPF_FC);
        WaveguideRight2.Reset(sampleRate, HPF_FC, LPF_FC);

        yn = 0.0f;

        // Seed the random number generator with the current time
        std::random_device rd;
        std::mt19937 gen(rd());

 

        // Create a uniform_real_distribution
        std::uniform_real_distribution<double> distribution(minDelaySeconds, maxDelaySeconds);
 

        WaveguideLeft1.SetParameters(1.0, distribution(gen));
        WaveguideRight1.SetParameters(1.0, distribution(gen));
        WaveguideLeft2.SetParameters(1.0, distribution(gen));
        WaveguideRight2.SetParameters(1.0, distribution(gen));

        //// Delay time numbers pregenerated to avoid differences in left and right channel reverb
        //WaveguideLeft1.SetParameters(1.0, 0.0478968);
        //WaveguideRight1.SetParameters(1.0, 0.0497891);
        //WaveguideLeft2.SetParameters(1.0, 0.048012);
        //WaveguideRight2.SetParameters(1.0, 0.042274);
    };
      
private:

    WaveguideUnit WaveguideLeft1, WaveguideRight1, WaveguideLeft2, WaveguideRight2;

    float junctionleft = 0.0f;
    float junctionright = 0.0f;
    float reflectionleft = 0.0f;
    float reflectionright = 0.0f;
    float reflectionleft2 = 0.0f;
    float reflectionright2 = 0.0f;

    float yn = 0.0f;

};

class WaveguideSpringReverbNetwork
{

public:

    WaveguideSpringReverbNetwork() {};
    ~WaveguideSpringReverbNetwork() {};

    float ProcessSample(float xn)
    {
        return ((One.ProcessSample(xn)) + (Two.ProcessSample(xn)) + (Three.ProcessSample(xn)) + (Four.ProcessSample(xn)) + (Five.ProcessSample(xn)) + (Six.ProcessSample(xn)) + (Seven.ProcessSample(xn)) + (Eight.ProcessSample(xn)));
        
    };

    void Reset(float sampleRate)
    {
        float minDelayTime = 0.03;
        float maxDelayTime = 0.04;

        One.Reset(sampleRate, minDelayTime, maxDelayTime, 200.f, 2000.f);
        Two.Reset(sampleRate, minDelayTime, maxDelayTime, 1000.f, 2500.f);
        Three.Reset(sampleRate, minDelayTime, maxDelayTime, 200.f, 2000.f);
        Four.Reset(sampleRate, minDelayTime, maxDelayTime, 200.f, 3200.f);
        Five.Reset(sampleRate, minDelayTime, maxDelayTime, 100.f, 2000.f);
        Six.Reset(sampleRate, minDelayTime, maxDelayTime, 500.f, 300.f);
        Seven.Reset(sampleRate, minDelayTime, maxDelayTime, 200.f, 2200.f);
        Eight.Reset(sampleRate, minDelayTime, maxDelayTime, 100.f, 3000.f);
     
    };

private:
    
    WaveguideSpringReverb One, Two, Three, Four, Five, Six, Seven, Eight;

};