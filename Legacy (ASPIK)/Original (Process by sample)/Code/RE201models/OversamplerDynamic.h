#pragma once

#include <memory>
#include <cmath>

#include "dependancies/FirFilter.h"

// _______ ODE solver

class rk4thOrderODETapeMag
{



public:

	rk4thOrderODETapeMag() {}
	~rk4thOrderODETapeMag() {}


	// Init or reset function, applys solver order add reset to increase stability
	void init(float sampleRate, bool solverorder)
	{
		fs = sampleRate;

		T = 1.f / fs;

		Mn1 = 0.0f;
		Hn1 = 0.0f;
		Hdn1 = 0.0f;

		secondorFourth = solverorder;
		

	}

	// Process sample with 4th order Runge Kutta solver
	float process4th(float xn, float saturation, float drive)
	{

		M_s = 0.5 + 1.5 * (1.0 - saturation);
		a = M_s / (0.01 + 6.0 * drive);
		c = std::sqrt(1.0f - W) - 0.01;
		k = 0.47875;


		float Hn = wireN * E * xn / g;

		float Hd = deriv(Hn, Hn1, Hdn1);

		float k1 = T * f(Mn1, Hn1, Hdn1);
		float k2 = T * f(Mn1 + (k1 * 0.5f), (Hn + Hn1) * 0.5f, (Hd + Hdn1) * 0.5f);
		float k3 = T * f(Mn1 + (k2 * 0.5f), (Hn + Hn1) * 0.5f, (Hd + Hdn1) * 0.5f);
		float k4 = T * f(Mn1 + k3, Hn, Hd);

		float Mn = Mn1 + (k1 / 6) + (k2 / 3) + (k3 / 3) + (k4 / 6);

		Mn1 = Mn;
		Hn1 = Hn;
		Hdn1 = Hd;

		bool illCondition = std::isnan(Mn) || Mn > upperLim;
		Mn = illCondition ? 0.0 : Mn;
		Hd = illCondition ? 0.0 : Hd;

		return (wireN * W * E * tapespeed * u0 * g * Mn) * 150;

	}

	// Process sample with 2nd order Runge Kutta solver
	float process2nd(float xn, float saturation, float drive) {
	
		M_s = 0.5 + 1.5 * (1.0 - saturation);
		a = M_s / (0.01 + 6.0 * drive);
		c = std::sqrt(1.0f - W) - 0.01;
		k = 0.47875;
	
		float Hn = wireN * E * xn / g;

		float Hd = deriv(Hn, Hn1, Hdn1);


		float k1 = T * f(Mn1, Hn1, Hdn1);
		float k2 = T * f(Mn1 + (k1 * 0.5), (Hn + Hn1) * 0.5, (Hd + Hdn1) * 0.5);

		float Mn = Mn1 + k2;

		Mn1 = Mn;
		Hn1 = Hn;
		Hdn1 = Hd;

		bool illCondition = std::isnan(Mn) || Mn > upperLim;
		Mn = illCondition ? 0.0 : Mn;
		Hd = illCondition ? 0.0 : Hd;

		return (wireN * W * E * tapespeed * u0 * g * Mn) * 150;
	
	}

	// Process sample function
	float process(float xn, float saturation, float drive) {
	
		float output = (secondorFourth == false) ? process2nd(xn, saturation, drive) : process4th(xn, saturation, drive);

		return output;
	
	}

private:

	// Langevin function
	float L(float x) {

		float coth = 1 / std::tanh(x);

		if (std::abs(x) > 10e-4) {
			return coth - (1 / x);
		}
		else {
			return (x / 3);
		}
	}

	// Langevin derivative
	float L_d(float x) {

		float xsquared = x * x;

		float cothsquared = (1 / std::tanh(x)) * (1 / std::tanh(x));

		if (std::abs(x) > 10e-4) {
			return (1 / xsquared) - cothsquared + 1;
		}
		else {
			return (1.f / 3);
		}

	}


	// Alpha transform derivative approximation
	float deriv(float x_n, float x_n1, float xDeriv_n1) {


		const float dAlpha = 0.75;
		return ((((float)1.0 + dAlpha) / T) * (x_n - x_n1)) - dAlpha * xDeriv_n1;
	}

	// ODE for calculating current magnetisation based on playhead position derivative
	float f(float M, float H, float H_d) {
	
		float Q = (H + alpha * M) / a;
		float M_diff = M_s * L(Q) - M;
		float delta = H_d > 0 ? 1 : -1;
		float delta_M = sign(delta) == sign(M_diff) ? 1 : 0;
		float L_prime = L_d(Q);

		float denominator = 1 - c * alpha * (M_s / a) * L_prime;

		float t1_num = (1 - c) * delta_M * M_diff;
		float t1_den = (1 - c) * delta * k - alpha * M_diff;
		float t1 = (t1_num / t1_den) * H_d;

		float t2 = c * (M_s / a) * H_d * L_prime;

		return (t1 + t2) / denominator;

	}

	// Returns the sign of a number
	int sign(int x) {

		if (x < 0)
			return -1;
		else if (x == 0)
			return 0;
		else if (x > 0)
			return 1;

	}


	float fs = 0.0f;
	float T = 0.0f; 
	float M_s = 1.f; 
	float a = M_s / 4.f; // Controls amount of saturation
	float alpha = 1.6e-3;
	float k = 0.47875; // Coercivity
	float c = 1.7e-1; 

	float g = 6.f; // Head gap
	float wireN = 100.f; // Wire turns
	float E = 0.1; // Head efficiency
	float tapespeed = 15;
	float W = 0.125;
	float u0 = 1.25663706e-6;


	// state variables
	float Mn1 = 0.0f;
	float Hn1 = 0.0f;
	float Hdn1 = 0.0f;

	// bool which implements order of differential calculation
	bool secondorFourth = false; 

	float upperLim = 20.f;



};

// Oversampler class for ODE tape mag
class OversamplerDynamic {

public:

    OversamplerDynamic() {}
    ~OversamplerDynamic() {}

	// Reset oversampler
    void reset(float sampleRate, unsigned int newosamount, bool solverorder) {

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

		float upsr = sampleRate * oversampleamount;


        intfilter.setCutoffFrequency(20000.f, upsr);
        decfilter.setCutoffFrequency(20000.f, upsr);
		tapemag.init(upsr, solverorder);

 


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
                oversampledbuffer[i] = decfilter.filter(tapemag.process(intfilter.filter(oversampledbuffer[i]), 15.f, 30.f));
           

            for (int i = 0; i < sampleframes; i++)
                outputbuffer[i] = oversampledbuffer[i * oversampleamount];

        }

        buffercounter += 1;
        buffercounter %= sampleframes;

        return yn;

    }


private:

	// Oversampler Parameters
	unsigned int oversampleamount = 4;
	const unsigned int sampleframes = 128;
	unsigned int oversampledbuffersize = oversampleamount * sampleframes;

	// Modulo counter for buffers
    unsigned int buffercounter = 0;
	
	// Buffer full check
    bool bufferfull = false;

	// Unique ptr arrays for circular buffer
    std::unique_ptr<float []> inputbuffer = nullptr;
    std::unique_ptr<float []> oversampledbuffer = nullptr;
    std::unique_ptr<float []> outputbuffer = nullptr;

	// Decimation filter taps 
    const int filtertaps = 100;

	// Interpolation and decimation filter objects
    FirFilter intfilter{ 50 };
    FirFilter decfilter { filtertaps };

	rk4thOrderODETapeMag tapemag;

};