//
//  BiquadCascade.h
//  JESEverTwo_UNIVERSAL
//
//  Created by Jordan  on 11/08/2023.
//

#ifndef BiquadCascade_h
#define BiquadCascade_h

#include <vector>
#include "Biquad.h"  // Include your biquad filter header file here

class BiquadCascade
{
public:
    BiquadCascade(int numFilters)
    {
        NumFilters = numFilters;
        
        if (numFilters <= 0) {
            throw std::invalid_argument("Number of filters must be greater than 0.");
        }
        
        filters.reserve(numFilters);
        for (int i = 0; i < numFilters; ++i) {
            filters.push_back(Biquad()); 
        }
    }

    
    void initFilters(float sampleRate, float cutoffFrequency)
    {
        
        for (int i = 0; i < NumFilters; ++i)
        {
            filters[i].Init(LPF, cutoffFrequency, sampleRate, 0.707f, 0.0f);
            filters[i].Reset(sampleRate);
        }
        
    }
    
    double process(double input) {
        double output = input;
        for (auto& filter : filters) {
            output = filter.ProcessSample(output);
        }
        return output;
    }
    
private:
    
    std::vector<Biquad> filters;
    
    int NumFilters = 0;
};

#endif /* BiquadCascade_h */
