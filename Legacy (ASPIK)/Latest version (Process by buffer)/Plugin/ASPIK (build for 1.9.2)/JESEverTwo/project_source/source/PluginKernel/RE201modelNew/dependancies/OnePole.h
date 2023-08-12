/*
  ==============================================================================

    OnePole.h
    Created: 1 Dec 2022 10:02:41am
    Author:  Jordan Evans

  ==============================================================================
*/

#ifndef OnePole_h
#define OnePole_h
#define M_PI 3.1415926535897932384626433


#include <math.h>

class OnePole {

public:

    OnePole() {};
    ~OnePole(){};
    void setFc(double Fc);
    void setAlpha(float time);
    float process(float xn);
    void reset(float sr){zminus1 = 0.0f; Alpha = 0.0f; SampleRate = sr;};
    
protected:
    
    float Alpha = 0.0f;
    float zminus1 = 0.0f;
    float SampleRate = 0.0f;
};

inline void OnePole::setFc(double Fc) {
    Alpha = 1 - exp(-2.0 * M_PI * Fc);
}

// This function is here so that this can be used as a timing filter for either compression timing or parameter smoothing, time is in seconds
inline void OnePole::setAlpha(float time) {
    
    float exponent = (1 / (time * SampleRate));

    Alpha = 1 - exp(-exponent);
    
}

inline float OnePole::process(float xn) {
    
    float yn = zminus1 + Alpha * (xn - zminus1);
    
    zminus1 = yn;
    
    return yn;
    
}


#endif
