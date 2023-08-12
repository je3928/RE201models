// Biquad Filter
// Author: Jordan Evans
// Date: 20/06/2023

#pragma once

#include <cmath>
#define Pi 3.1415926535897932384626433

enum BiquadType
{
    LPF = 0,
    HPF = 1,
    Notch = 2,
    Peaking = 3,
    LowShelf = 4,
    HighShelf = 5
};


class Biquad
{

    public:

    // Ctor
    Biquad();
    // Dtor
    ~Biquad();  

    // Initialise filter with specified parameters, call to change filter type 
    // Only peaking and shelving filters require gain so set to 0.0f when NOT using those types.
    void Init(BiquadType filterType, float fc, float fs, float Q, float gain_dB);

    // Call to set all parameters without changing filter type 
    void SetParameters(float fc, float Q, float gain_dB);

    // Call to only set frequency cutoff
    void SetFc(float fc);

    // Call to only set gain. Only peaking and shelving filters require gain
    // so set to 0.0f when NOT using those types.
    void SetGain(float gain_dB);

    // Reset filter
    void Reset(float fs);

    // Process a sample with the filter
    float ProcessSample(float xn);
    
    private:

    // Feedforward coeffs
    float a0 = 0.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;

    // Feedback coeffs
    float b0 = 0.0f;
    float b1 = 0.0f;
    float b2 = 0.0f;
    
    // State variables
    float xminusone = 0.0f;
    float xminustwo = 0.0f;
    float yminusone = 0.0f;
    float yminustwo = 0.0f;

    // Parameters
    float CutoffFrequency = 0.0f;
    float SampleRate = 0.0f;
    float QualityFactor = 0.0f;
    float Gain_dB = 0.0f;
    int CurrentType = 0;

    // Calculate filter for current parameters
    void CalcFilter()
    {
        // Omega: Angular frequency
        float w = 2 * Pi * (CutoffFrequency / SampleRate);

        // Cos(Omega)
        float cosw = cos(w);

        // Sin(Omega)
        float sinw = sin(w);

        // Damping Coefficient
        float a = sinw / (2 * QualityFactor);

        // Gain for shelving and peaking filters
        float A = pow(10, (Gain_dB / 40.f));

        // Variable for shelving filter coeff calculation (simplifies calculations)
        float var2sqAa = 2 * sqrt(A) * a;

        switch(CurrentType)
        {
            case LPF:

            // Compute the feedforward coefficients 'b'
            b0 = (1.f - cosw) / 2.f;
            b1 = 1.f - cosw;
            b2 = (1.f - cosw) / 2.f;

            // Compute the feedback coefficients 'a'
            a0 = 1.f + a;
            a1 = -2.f * cosw;
            a2 = 1.f - a;

            break;

            case HPF:

            // Compute the feedforward coefficients 'b'
            b0 = (1.f + cosw) / 2.f;
            b1 = -(1.f + cosw);
            b2 = (1.f + cosw) / 2.f;

            // Compute the feedback coefficients 'a'
            a0 = 1.f + a;
            a1 = -2.f * cosw;
            a2 = 1.f - a;

            break;

            case Notch:

            // Compute the feedforward coefficients 'b'
            b0 = 1.f;
            b1 = -2.f * cosw;
            b2 = 1.f;

            // Compute the feedback coefficients 'a'
            a0 = 1.f + a;
            a1 = -2.f * cosw;
            a2 = 1.f - a;

            break;

            case Peaking:

            // Compute the feedforward coefficients 'b'
            b0 = 1.f + a * A;
            b1 = -2.f * cosw;
            b2 = 1.f - a * A;

            // Compute the feedback coefficients 'a'
            a0 = 1.f + (a / A);
            a1 = -2.f * cosw;
            a2 = 1.f - (a / A);

            break;

            case LowShelf:

            // Compute the feedforward coefficients 'b'
            b0 = A * ((A + 1.f) - (A - 1.f) * cosw + var2sqAa);
            b1 = 2.f * A * ((A - 1.f) - (A + 1.f) * cosw);
            b2 = A * ((A + 1.f) - (A - 1.f) * cosw - var2sqAa);

            // Compute the feedback coefficients 'a'
            a0 = (A + 1.f) + (A - 1.f) * cosw + var2sqAa;
            a1 = (-2.f) * ((A - 1.f) + (A + 1.f) * cosw);
            a2 = (A + 1.f) + (A - 1.f) * cosw - var2sqAa;

            break;

            case HighShelf:

            break;
        }


    }

};
