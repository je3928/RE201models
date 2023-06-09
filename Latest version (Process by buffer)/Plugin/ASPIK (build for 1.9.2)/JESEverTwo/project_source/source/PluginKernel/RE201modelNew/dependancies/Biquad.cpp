//
//  Biquad.cpp
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//

#define M_PI 3.1415926535897932384626433

#include <cmath>
#include "Biquad.h"

Biquad::Biquad() {
    type = bq_type_lowpass;
    a0 = 1.0;
    a1 = a2 = b1 = b2 = 0.0;
    fc = 0.50;
    fs = 0.0000000;
    q = 0.707;
    peakGain = 0.0;
    s1 = s2 = s1minus1 = s2minus1 = 0.0;
}

Biquad::Biquad(int type, double Fc, double Q, double peakGainDB) {
    setBiquad(type, Fc, Q, peakGainDB);
    s1 = s2 = s1minus1 = s2minus1 = 0.0;
}

Biquad::~Biquad() {
}

void Biquad::setType(int type) {
    this->type = type;
    calcBiquad();
}

void Biquad::setQ(double Q) {
    this->q = Q;
    calcBiquad();
}

void Biquad::setFc(double Fc) {
    this->fc = Fc;
    calcBiquad();
}

void Biquad::setPeakGain(double peakGainDB) {
    this->peakGain = peakGainDB;
    calcBiquad();
}

void Biquad::setBiquad(int type, double Fc, double Q, double peakGainDB) {
    this->type = type;
    this->q = Q;
    this->fc = Fc;
    setPeakGain(peakGainDB);
}

void Biquad::calcBiquad(void) {
    double norm;
    double V = pow(10, fabs(peakGain) / 20.0);
    double K = tan(M_PI * fc);
    switch (this->type) {
    case bq_type_lowpass:
        norm = 1 / (1 + K / q + K * K);
        a0 = K * K * norm;
        a1 = 2 * a0;
        a2 = a0;
        b1 = 2 * (K * K - 1) * norm;
        b2 = (1 - K / q + K * K) * norm;
        break;

    case bq_type_highpass:
        norm = 1 / (1 + K / q + K * K);
        a0 = 1 * norm;
        a1 = -2 * a0;
        a2 = a0;
        b1 = 2 * (K * K - 1) * norm;
        b2 = (1 - K / q + K * K) * norm;
        break;

    case bq_type_bandpass:
        norm = 1 / (1 + K / q + K * K);
        a0 = K / q * norm;
        a1 = 0;
        a2 = -a0;
        b1 = 2 * (K * K - 1) * norm;
        b2 = (1 - K / q + K * K) * norm;
        break;

    case bq_type_notch:
        norm = 1 / (1 + K / q + K * K);
        a0 = (1 + K * K) * norm;
        a1 = 2 * (K * K - 1) * norm;
        a2 = a0;
        b1 = a1;
        b2 = (1 - K / q + K * K) * norm;
        break;

    case bq_type_peak:
        if (peakGain >= 0) {    // boost
            norm = 1 / (1 + 1 / q * K + K * K);
            a0 = (1 + V / q * K + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = (1 - V / q * K + K * K) * norm;
            b1 = a1;
            b2 = (1 - 1 / q * K + K * K) * norm;
        }
        else {    // cut
            norm = 1 / (1 + V / q * K + K * K);
            a0 = (1 + 1 / q * K + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = (1 - 1 / q * K + K * K) * norm;
            b1 = a1;
            b2 = (1 - V / q * K + K * K) * norm;
        }
        break;
    case bq_type_lowshelf:
        if (peakGain >= 0) {    // boost
            norm = 1 / (1 + sqrt(2) * K + K * K);
            a0 = (1 + sqrt(2 * V) * K + V * K * K) * norm;
            a1 = 2 * (V * K * K - 1) * norm;
            a2 = (1 - sqrt(2 * V) * K + V * K * K) * norm;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - sqrt(2) * K + K * K) * norm;
        }
        else {    // cut
            norm = 1 / (1 + sqrt(2 * V) * K + V * K * K);
            a0 = (1 + sqrt(2) * K + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = (1 - sqrt(2) * K + K * K) * norm;
            b1 = 2 * (V * K * K - 1) * norm;
            b2 = (1 - sqrt(2 * V) * K + V * K * K) * norm;
        }
        break;
    case bq_type_highshelf:
        if (peakGain >= 0) {    // boost
            norm = 1 / (1 + sqrt(2) * K + K * K);
            a0 = (V + sqrt(2 * V) * K + K * K) * norm;
            a1 = 2 * (K * K - V) * norm;
            a2 = (V - sqrt(2 * V) * K + K * K) * norm;
            b1 = 2 * (K * K - 1) * norm;
            b2 = (1 - sqrt(2) * K + K * K) * norm;
        }
        else {    // cut
            norm = 1 / (V + sqrt(2 * V) * K + K * K);
            a0 = (1 + sqrt(2) * K + K * K) * norm;
            a1 = 2 * (K * K - 1) * norm;
            a2 = (1 - sqrt(2) * K + K * K) * norm;
            b1 = 2 * (K * K - V) * norm;
            b2 = (V - sqrt(2 * V) * K + K * K) * norm;
        }
        break;
    }

    return;
}