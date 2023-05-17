#include <memory>
#include <cmath>

#include "dependancies/FirFilter.h"

// Oversampler class for erf saturation
class OversamplerStatic {

public:

    OversamplerStatic() {}
    ~OversamplerStatic() {}

    // Reset oversampler
    void reset(float sampleRate, int newosamount) {

        oversampleamount = newosamount;
        oversampledbuffersize = oversampleamount * sampleframes;

        inputbuffer.reset(new float[sampleframes]);
        oversampledbuffer.reset(new float[oversampledbuffersize]);
        outputbuffer.reset(new float[sampleframes]);

        buffercounter = 0;
        bufferfull = false;


        for (int i = 0; i < sampleframes; i++) {

            inputbuffer[i] = 0.0f;
            outputbuffer[i] = 0.0f;


        }

        for (int i = 0; i < oversampledbuffersize; i++) {


            oversampledbuffer[i] = 0.0f;


        }

        intfilter.setCutoffFrequency(20000.f, (sampleRate * oversampleamount));
        decfilter.setCutoffFrequency(20000.f, sampleRate * oversampleamount);

 


    }

    // Sets oversampler amount
    void setOSamount(unsigned int osamount) {
    
        newosamount = osamount;

    
    }

    // Process sample 
    float processSample(float xn) {

        inputbuffer[buffercounter] = xn;
        float yn = outputbuffer[buffercounter];
        

        bufferfull = (buffercounter == (sampleframes - 1)) ? true : false;

        if (bufferfull == true) {

            for (int i = 0; i < sampleframes; i++) {

                for (int j = 0; j < oversampleamount; j++)
                     oversampledbuffer[(i * oversampleamount) + j] = inputbuffer[i];

            }

            for (int i = 0; i < oversampledbuffersize; i++)
                oversampledbuffer[i] = decfilter.filter(erf(intfilter.filter(oversampledbuffer[i])));
           

            for (int i = 0; i < sampleframes; i++)
                outputbuffer[i] = oversampledbuffer[i * oversampleamount];

        }

        buffercounter += 1;
        buffercounter %= sampleframes;

        return yn;

    }


private:

    // Oversampler parameters
    unsigned int oversampleamount = 4;
    const unsigned int sampleframes = 128;
    unsigned int oversampledbuffersize = oversampleamount * sampleframes;

    // Modulo counter for buffers
    unsigned int buffercounter = 0;

    // Buffer full check
    bool bufferfull = false;

    // Unique ptr arrays for buffers
    std::unique_ptr<float []> inputbuffer = nullptr;
    std::unique_ptr<float []> oversampledbuffer = nullptr;
    std::unique_ptr<float []> outputbuffer = nullptr;

    // decimation filtertaps
    const int filtertaps = 100;

    // Interpolation and decimation filters
    FirFilter intfilter{ 50 };
    FirFilter decfilter { filtertaps };

};