#pragma once
#include <cmath>

class rk4thOrderODETapeMag
{



public:

    rk4thOrderODETapeMag() {}
    ~rk4thOrderODETapeMag() {}


    // Init or reset function, applys solver order at reset to increase stability
    void init(float sampleRate)
    {
        fs = sampleRate;

        T = 1.f / fs;

        Mn1 = 0.0f;
        Hn1 = 0.0f;
        Hdn1 = 0.0f;
        

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

        float HnHalfSample = (Hn + Hn1) * 0.5f;
        float HdHalfSample = (Hd + Hdn1) * 0.5f;

        float k1 = T * f(Mn1, Hn1, Hdn1);
        float k2 = T * f(Mn1 + (k1 * 0.5f), HnHalfSample, HdHalfSample);
        float k3 = T * f(Mn1 + (k2 * 0.5f), HnHalfSample, HdHalfSample);
        float k4 = T * f(Mn1 + k3, Hn, Hd);

        float Mn = Mn1 + (k1 * onesixth) + (k2 * onethird) + (k3 * onethird) + (k4 * onesixth);

        Mn1 = Mn;
        Hn1 = Hn;
        Hdn1 = Hd;

        bool illCondition = std::isnan(Mn) || Mn > upperLim;

        if (illCondition)
            Mn = Hd = 0.0f;


        return (wireN * W * E * tapespeed * u0 * g * Mn) * 150;

    }

    // Process sample with 2nd order Runge Kutta solver
    float process2nd(float xn, float saturation, float drive)
    {
    
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

        if (illCondition)
            Mn = Hd = 0.0f;


        return (wireN * W * E * tapespeed * u0 * g * Mn) * 150;
    
    }


private:

    // Langevin function
    float L(float x)
    {

        float coth = 1 / std::tanh(x);

        if (std::abs(x) > 10e-4) {
            return coth - (1 / x);
        }
        else {
            return (x / 3.f);
        }
    }

    // Langevin derivative
    float L_d(float x)
    {

        float xsquared = x * x;

        float cothsquared = (1 / std::tanh(x)) * (1 / std::tanh(x));

        if (std::abs(x) > 10e-4) {
            return (1 / xsquared) - cothsquared + 1;
        }
        else {
            return (1.f / 3.f);
        }

    }


    // Alpha transform derivative approximation
    float deriv(float x_n, float x_n1, float xDeriv_n1)
    {


        const float dAlpha = 0.75;
        return ((((float)1.0 + dAlpha) / T) * (x_n - x_n1)) - dAlpha * xDeriv_n1;
    }

    // ODE for calculating current magnetisation based on playhead position derivative
    float f(float M, float H, float H_d)
    {
    
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
    int sign(int x)
    {

        if (x < 0)
            return -1;
        else if (x == 0)
            return 0;
        else if (x > 0)
            return 1;
        else
            return 0;

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

    float upperLim = 20.f;


    // Fractions, for speed....

    float onesixth = 0.166666666667;
    float onethird = 0.333333333333;


};
