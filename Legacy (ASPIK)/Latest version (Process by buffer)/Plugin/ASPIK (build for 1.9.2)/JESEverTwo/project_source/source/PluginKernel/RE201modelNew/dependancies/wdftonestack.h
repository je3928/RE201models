#pragma once

// WDFToneStack
// Author: Jordan Evans
// Date: 09/02/2023
//
// Implements a wdf model of a modified 5F6-A tone stack. The filter had been designed to model the frequency response of the RE-201 tone stack.  

#define M_PI 3.1415926535897932384626433

#include "chowdsp_wdf/chowdsp_wdf.h"
using namespace chowdsp::wdft;

// High shelf
class WDFHighShelf {



public:

    WDFHighShelf() {};
    ~WDFHighShelf() {};


    // prepare the WDF model here...
    void prepare(double sampleRate) {

        fs = sampleRate;

        cap2.prepare(sampleRate);
        cap1.prepare(sampleRate);


    }

    void updateParams(double Cutoff, double g) {

        cutoff = Cutoff;

        G = g;
        C = 1 / (2 * M_PI * cutoff * R);

    }

    // use the WDF model to process one sample of data
    inline double processSample(double x) {

        vs.setVoltage(x);

        vs.incident(ser1.reflected());
        ser1.incident(vs.reflected());

        vs2.setVoltage(x);

        vs2.incident(ser2.reflected());
        ser2.incident(vs2.reflected());

        float hpout = voltage<float>(cap1);
        float lpout = voltage<float>(res2);




        //return (lpout * 1) + (hpout * G);

        return (hpout * 1) + (lpout * G);

    }


private:

    float cutoff = 3000.f;
    const float R = 100000;
    float C = 1 / (2 * M_PI * cutoff * R);

    ResistorT<float> res1{ R };
    CapacitorT<float> cap1{ C, 48000.f };


    WDFSeriesT<float, decltype(res1), decltype(cap1)> ser1{ res1, cap1 };

    IdealVoltageSourceT<float, decltype(ser1)> vs{ ser1 };

    ResistorT<float> res2{ R };
    CapacitorT<float> cap2{ C, 48000.f };


    WDFSeriesT<float, decltype(res2), decltype(cap2)> ser2{ res2, cap2 };

    IdealVoltageSourceT<float, decltype(ser2)> vs2{ ser2 };




    float fs = 0.0f;

    float G = 0.0f;


};

// Low shelf
class WDFLowShelf {



public:

    WDFLowShelf() {};
    ~WDFLowShelf() {};


    // prepare the WDF model here...
    void prepare(double sampleRate) {

        fs = sampleRate;

        cap2.prepare(sampleRate);
        cap1.prepare(sampleRate);


    }

    void updateParams(double Cutoff, double g) {

        cutoff = Cutoff;

        G = g;

        C = 1 / (2 * M_PI * cutoff * R);
    }

    // use the WDF model to process one sample of data
    inline double processSample(double x) {

        vs.setVoltage(x);

        vs.incident(ser1.reflected());
        ser1.incident(vs.reflected());

        vs2.setVoltage(x);

        vs2.incident(ser2.reflected());
        ser2.incident(vs2.reflected());

        float hpout = voltage<float>(cap1);
        float lpout = voltage<float>(res2);




        //return (lpout * 1) + (hpout * G);

        return (hpout * G) + (lpout * 1);

    }


private:

    float cutoff = 200.f;
    const float R = 100000;
    float C = 1 / (2 * M_PI * cutoff * R);

    ResistorT<float> res1{ R };
    CapacitorT<float> cap1{ C, 48000.f };


    WDFSeriesT<float, decltype(res1), decltype(cap1)> ser1{ res1, cap1 };

    IdealVoltageSourceT<float, decltype(ser1)> vs{ ser1 };

    ResistorT<float> res2{ R };
    CapacitorT<float> cap2{ C, 48000.f };


    WDFSeriesT<float, decltype(res2), decltype(cap2)> ser2{ res2, cap2 };

    IdealVoltageSourceT<float, decltype(ser2)> vs2{ ser2 };

    float fs = 0.0f;

    float G = 0.0f;



};


// Tone stack implementing a low shelf and high shelf
class WDFToneStack {



public:

    WDFToneStack() {};
    ~WDFToneStack() {};


    // prepare the WDF model here...
    void prepare(double sampleRate) {


        ls.prepare(sampleRate);
        hs.prepare(sampleRate);

    }

    void updateParams(double low, double high) {


        ls.updateParams(200.f, low);
        hs.updateParams(3000.f, high);



    }
    // use the WDF model to process one sample of data
    inline double processSample(double x) {

        float yn = ls.processSample(x);

        yn = hs.processSample(yn);

        return yn * 0.25;

    }


private:

    WDFLowShelf ls;
    WDFHighShelf hs;

};
