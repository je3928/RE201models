/*
  ==============================================================================

    AllPassFilterTransposedDirect2.h
    Created: 10 Nov 2022 10:33:59am
    Author:  je050

  ==============================================================================
*/

#include "CircularBuffer.h"

class AllPassFilter
{

public:

    AllPassFilter() {};
    ~AllPassFilter() {};

    void Init(float sr)
    {

        SampleRate = sr;
        xminusone = yminusone = 0.0f;

    };

    float ProcessSample(float xn) 
    {
      
        float yn = xminusone + Alpha * (yminusone - xn);

        xminusone = xn;
        yminusone = yn;

        return yn;
    };



private:

    float SampleRate = 0.0f;
    float Alpha = 0.5f;

    float xminusone = 0.0f;
    float yminusone = 0.0f;

};
