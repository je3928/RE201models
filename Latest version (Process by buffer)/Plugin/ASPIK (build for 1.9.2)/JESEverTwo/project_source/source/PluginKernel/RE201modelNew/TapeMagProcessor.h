#pragma once

#include "dependancies/tapemag.h"
#include "dependancies/FirFilter.h"
#include <vector>
#include <memory>



class TapeMagProcessor
{

public:

    TapeMagProcessor();
    ~TapeMagProcessor();

    void Reset(float sampleRate, int numChannels, int osamount);
    void ProcessBuffer(std::vector<std::vector<float>>& buffer, int blockSize);


private:

    // Oversampling filter parameters
    static const int upsampleCoeffs = 100;
    static const int downsampleCoeffs = 100;
    const float transitionBW = 0.1;

    // DSP objects
    std::vector<std::unique_ptr<rk4thOrderODETapeMag>> TapeODESolvers;
    std::vector<std::unique_ptr<FirFilter>> InterpolationFilters;
    std::vector<std::unique_ptr<FirFilter>> DecimationFilters;




    // Object variables
    float SampleRate = 0.0f;
    int NumChannels = 2;
    int OSamount = 2;
    int OSiterations = 1;



};