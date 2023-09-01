#pragma once

#include "CircularBuffer.h"
#include "AllPassFilter.h"
#include "Biquad.h"


class WaveguideUnit 
{
public:

    WaveguideUnit();
    ~WaveguideUnit();

    float ProcessSample(float xn);
    void Reset(float sampleRate);
    void SetParameters(float RT60, float delayTimeSeconds);



private:

    float sampleRate = 0.0f;
    float rt60 = 0.0f;
    float delayTimeSeconds = 0.0f;
    float attenuation = 0.99f;

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

    void Reset(float sampleRate) 
    {
        WaveguideLeft1.Reset(sampleRate);
        WaveguideRight1.Reset(sampleRate);
        WaveguideLeft2.Reset(sampleRate);
        WaveguideRight2.Reset(sampleRate);

        yn = 0.0f;

        WaveguideLeft1.SetParameters(1.0, 0.02);
        WaveguideRight1.SetParameters(1.0, 0.08);
        WaveguideLeft2.SetParameters(1.0, 0.06);
        WaveguideRight2.SetParameters(1.0, 0.03);
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