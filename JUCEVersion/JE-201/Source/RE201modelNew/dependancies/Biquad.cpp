// Biquad Filter
// Author: Jordan Evans
// Date: 20/06/2023

#include "Biquad.h"

Biquad::Biquad()
{

}

Biquad::~Biquad()
{

}

void Biquad::Init(BiquadType filterType, float fc, float fs, float Q, float gain_dB)
{
    // Save variables
    CurrentType = filterType;
    CutoffFrequency = fc;
    SampleRate = fs;
    QualityFactor = Q;
    Gain_dB = gain_dB;

    // Calculate filter coefficients
    CalcFilter();
}

void Biquad::SetParameters(float fc, float Q, float gain_dB)
{  
    // Only recalculate if a parameter has changed
    if(fc != CutoffFrequency || Q != QualityFactor || gain_dB != Gain_dB)
    {
        // Save parameters
        CutoffFrequency = fc;
        QualityFactor = Q;
        Gain_dB = gain_dB;

        // Calculate filter coefficients
        CalcFilter();
    }

}

void Biquad::SetFc(float fc)
{
    // Only recalculate if cutoff frequency changed
    if(fc != CutoffFrequency)
    {
        // Save cutoff frequency
        CutoffFrequency = fc;

        // Calculate filter coefficients
        CalcFilter();
    }
}

void Biquad::SetGain(float gain_dB)
{
    // Only recalculate if gain has changed
    if(gain_dB != Gain_dB)
    {
        // Save gain
        Gain_dB = gain_dB;

        // Calculate filter coefficients
        CalcFilter();
    }
}


void Biquad::Reset(float fs)
{
    // Reset filter state variables
    yminustwo = yminusone = xminustwo = xminusone = 0.0f;

    // Only recalculate if sample rate changed
    if (fs != SampleRate)
    {
    // Save sample rate
    SampleRate = fs;

    // Calculate filter coefficients
    CalcFilter();
    }
}

float Biquad::ProcessSample(float xn)
{
    // Calculate current output sample
    float yn = (b0 / a0) * xn + (b1 / a0) * xminusone + (b2 / a0) * xminustwo - (a1 / a0) * yminusone - (a2 / a0) * yminustwo;

    // Update input state variables
    xminustwo = xminusone;
    xminusone = xn;

    // Update output state variables
    yminustwo = yminusone;
    yminusone = yn;

    // Return current output sample
    return yn;
}
