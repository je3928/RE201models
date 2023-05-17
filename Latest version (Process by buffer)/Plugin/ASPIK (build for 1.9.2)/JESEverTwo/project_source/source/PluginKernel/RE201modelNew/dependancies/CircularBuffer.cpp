/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 3 Nov 2022 10:13:02am
    Author:  Jordan Evans

  ==============================================================================
*/

#include "CircularBuffer.h"

// C-tor
CircularBuffer::CircularBuffer(){};
// D-tor
CircularBuffer::~CircularBuffer(){};



// Function to reset all buffer values to zero
void CircularBuffer::ClearBuffer(){
    
    // iterate through buffer
    for (int i = 0; i < buffer_length; i++)
    {
        
        // set to zero
        delaybuffer[i] = 0.0f;
        
    }
    
}

// Function to initialise the buffer using the sample rate and the maximum desired delay time.
void CircularBuffer::Init(int sr, float max_delay_time_ms) {
    
    // Save variables into object
    SampleRate = sr;
    MaxDelayTimeMs = max_delay_time_ms;
    
    // Calcluate buffer length
    buffer_length = (unsigned int)((MaxDelayTimeMs / 1000) * SampleRate + 1);
    
    // Calculate buffer length to next power of 2
    buffer_length = (unsigned int)(pow(2, ceil(log(buffer_length) / log(2))));
    
    // Reset array with new buffer length
    delaybuffer.reset(new float[buffer_length]);
    
    // Initialise array values to zero
    ClearBuffer();
    
    // Initialise buffer write index
    WriteIndex = 0;
    
}

// Function that reads from the buffer with a desired amount of delay in ms. Calculates delay in samples using sample rate. Optional calculation of fractional delay using a linear interpolation function.
float CircularBuffer::BufferRead(float delay_time_ms, bool interpolate_line) {
    
    // Save variables to object
    interpolateline = interpolate_line;
        
    // Calculate both the normal and fractional delay in samples
    delay_time_samples = (delay_time_ms / 1000) * SampleRate;
    delay_fractional_samples = (delay_time_ms / 1000) * SampleRate;
    
    // Calculate read index
    int ReadIndex = (WriteIndex - 1) - delay_time_samples;

    // Wrap read index using modulo
    ReadIndex %=  buffer_length;
   
    // Calculate normal output
    float yn = delaybuffer[ReadIndex];
    
    // interpolation enabled check
    if (interpolateline == true) {

        // read one sample older
        int bReadIndex = ReadIndex - 1;

        // Wrap read index for second sample 
        bReadIndex %= buffer_length;
        
        // calculate input into interpolator
        float a = yn;
        float b = delaybuffer[bReadIndex];
        float t = delay_fractional_samples - delay_time_samples;
        
        // calculate fractional delay sample with a linear interpolator
        yn = lerp(a, b, t);
    }

    // return output
    return yn;
}


// Function to read from the buffer if delay required is explicitly in samples. By default does not use interpolation.
float CircularBuffer::BufferReadSamples(int delay_in_samples) {

    // Set interpolation off
    interpolateline = false;

    // Calculate read index
    int ReadIndex = (WriteIndex - 1) - delay_in_samples;

    // Wrap read index using modulo
    ReadIndex %= buffer_length;

    // Calculate normal output
    float yn = delaybuffer[ReadIndex];

    // return output
    return yn;
}


// Function to write to buffer
void CircularBuffer::BufferWrite(float xn) {
 
    // Increment write index
    WriteIndex++;
    
    // Wrap write index with modulo
    WriteIndex %= buffer_length;

    // Write sample to buffer
    delaybuffer[WriteIndex] = xn;
    
}




