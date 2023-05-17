/*
  ==============================================================================

    CircularBuffer.h
    Created: 3 Nov 2022 10:13:02am
    Author:  Jordan Evans

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <cmath>

 class CircularBuffer
{
    
public:
    
    // C-tor
    CircularBuffer();
    // D-tor
    ~CircularBuffer();
    
    
    //Public member functions
    void Init(int sr, float max_delay_time_ms);
    void ClearBuffer();
    void BufferWrite(float xn);
    float BufferRead(float delay_time_ms, bool interpolate_line);
    float BufferReadSamples(int delay_int_samples);
    
    
    
    
private:
    
    
    //Private member variables
    unsigned int WriteIndex = 0;
    float SampleRate = 0.0f;
    unsigned int buffer_length = 0;
    
    int delay_time_samples = 0;
    float delay_fractional_samples = 0.0f;
    
    float MaxDelayTimeMs = 0.0f;
    
    bool interpolateline = false; 
    
    // Unique ptr array to use as circular buffer, self deletes when goes out of scope.
    std::unique_ptr<float[]> delaybuffer = nullptr;
    

    
    // linear interpolation function
    float lerp(float a, float b, float f)
    {
        return a * (1.0f - f) + (b * f);
    }
    
};
