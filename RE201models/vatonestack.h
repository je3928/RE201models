#pragma once
// VAToneStack
//
// Author: Jordan Evans
// Date: 08/02/2023
//
// Implements the discretised 5F6-A tone stack algorithm developed by Yeh (2006). The component values and algorithm has been modified to resemble
// the frequency response of the RE-201 tone stack. The mid band has been removed from the algorithm.  

#include <cmath>

class VAToneStack
{

public:

// Reset filter
void reset(float sampleRate) {

SampleRate = sampleRate;
xnminus3 = 0.0f;
xnminus2 = 0.0f;
xnminus1 = 0.0f;

ynminus3 = 0.0f;
ynminus2 = 0.0f;
ynminus1 = 0.0f;

Low = 0.0f;
Top = 0.0f;



}

// Update parameters then calc coefficients
void updateFilterCoefficients(float low, float top) {

if(Low != low || Top != top) {    

	float midsquared = mid * mid;
	float R3squared = R3 * R3; 

b1 = top*C1*R1 + mid*C3*R3 + low*(C1*R2 + C2*R2) + (C1*R3 + C2*R3);
b2 = top*(C1*C2*R1*R4 + C1*C3*R1*R4) - midsquared*(C1*C3*R3squared + C2*C3*R3squared)
+ mid*(C1*C3*R1*R3 + C1*C3* R3squared + C2*C3* R3squared) +
low*(C1*C2*R1*R2 + C1*C2*R2*R4 + C1*C3*R2*R4) + 
low*mid*(C1*C3*R2*R3 + C2*C3*R2*R3) + (C1*C2*R1*R3 + C1*C2*R3*R4 + C1*C3*R3*R4);
b3 = low*mid*(C1*C2*C3*R1*R2*R3 + C1*C2*C3*R2*R3*R4) - midsquared *(C1*C2*C3*R1* R3squared + C1*C2*C3* R3squared *R4) + mid*(C1*C2*C3*R1* R3squared + C1*C2*C3* R3squared *R4) + top*C1*C2*C3*R1*R3*R4 - top*mid*C1*C2*C3*R1*R3*R4 + top*low*C1*C2*C3*R1*R2*R4;
a0 = 1;
a1 = (C1*R1 + C1*R3 + C2*R3 + C2*R4 + C3*R4) + mid*C3*R3 + low*(C1*R2 + C2*R2);
a2 = mid*(C1*C3*R1*R3 - C2*C3*R3*R4 + C1*C3* R3squared + C2*C3* R3squared) + low*mid*(C1*C3*R2*R3 + C2*C3*R2*R3) - midsquared *(C1*C3* R3squared + C2*C3* R3squared) + low*(C1*C2*R2*R4 + C1*C2*R1*R2 + C1*C3*R2*R4 + C2*C3*R2*R4) + (C1*C2*R1*R4 + C1*C3*R1*R4 + C1*C2*R3*R4 + C1*C2*R1*R3 + C1*C3*R3*R4 + C2*C3*R3*R4);
a3 = low*mid*(C1*C2*C3*R1*R2*R3 + C1*C2*C3*R2*R3*R4) - midsquared *(C1*C2*C3*R1* R3squared + C1*C2*C3* R3squared *R4) + mid*(C1*C2*C3* R3squared *R4 + C1*C2*C3*R1* R3squared - C1*C2*C3*R1*R3*R4) + low*C1*C2*C3*R1*R2*R4 + C1*C2*C3*R1*R3*R4;


float c = 2.f * SampleRate;

float csquared = c * c;
float ccubed = c * c * c;

B0 = -b1 * c - b2 * csquared - b3 * ccubed;
B1 = -b1 * c + b2 * csquared + 3 * b3 * ccubed;
B2 = b1 * c + b2 * csquared - 3 * b3 * ccubed;
B3 = b1 * c - b2* csquared + b3 * ccubed;
A0 = -a0 - a1*c - a2* csquared - a3* ccubed;
A1 = -3 * a0 - a1 * c + a2 * csquared + 3 * a3 * ccubed;
A2 = -3 * a0 + a1 * c + a2 * csquared - 3 * a3 * ccubed;
A3 = -a0 + a1 * c - a2 * csquared + a3 * ccubed;

}

Low = low;
Top = top; 

return;

}

// Process sample
float processSample(float xn) {

float yn = (B0 / A0) * xn + (B1 / A0) * xnminus1 + (B2 / A0) * xnminus2 + (B3 / A0) * xnminus3 - (A1 / A0) * ynminus1 - (A2 / A0) * ynminus2 - (A3 / A0) * ynminus3; 

xnminus3 = xnminus2;
xnminus2 = xnminus1;
xnminus1 = xn;

ynminus3 = ynminus2;
ynminus2 = ynminus1;
ynminus1 = yn;

return yn; 

}



private:

// ----- analogue coefficients

float b1 = 0.0f;
float b2 = 0.0f;
float b3 = 0.0f;
float a0 = 0.0f;
float a1 = 0.0f;
float a2 = 0.0f;
float a3 = 0.0f;


// ----- analogue coefficients
// ---------------------------
// ----- digital coefficients

float B0 = 0.0f; 
float B1 = 0.0f;
float B2 = 0.0f;
float B3 = 0.0f;
float A0 = 0.0f;
float A1 = 0.0f;
float A2 = 0.0f;
float A3 = 0.0f;

// ----- digital coefficients
// ---------------------------
// ----- component values
float C1 = 470e-12;
float C2 = 22e-9;
float C3 = 22e-9;
float R1 = 250000;
float R2 = 100000;
float R3 = 1000;
float R4 = 100000;
// ----- component values

const float mid = 0.5f;
float SampleRate = 0.0f;
float xnminus1 = 0.0f;
float xnminus2 = 0.0f;
float xnminus3 = 0.0f;
float ynminus1 = 0.0;
float ynminus2 = 0.0f;
float ynminus3 = 0.0f;

float Low = 0.0f, Top = 0.0f; 

};