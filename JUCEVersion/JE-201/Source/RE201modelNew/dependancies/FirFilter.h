#pragma once 

#include <cmath>
#include <vector>
#define M_PI 3.1415926535897932384626433


class FirFilter {
public:
	FirFilter(int numTaps) : m_numTaps(numTaps) {
		m_coeffs.resize(numTaps);
		m_delayLine.resize(numTaps);
	}

	void setCutoffFrequency(float cutoffFreq, float sampleRate) {
		float omega = 2.0 * M_PI * cutoffFreq / sampleRate;
		for (int i = 0; i < m_numTaps; i++) {
			if (i == m_numTaps / 2) {
				m_coeffs[i] = omega / M_PI;
			}
			else {
				float n = i - m_numTaps / 2.f;
				m_coeffs[i] = sin(omega * n) / (M_PI * n);
			}
		}
	}

	float filter(float input) {
		float output = 0.0;
		m_delayLine[m_currentTap] = input;
		for (int i = 0; i < m_numTaps; i++) {
			int index = (m_currentTap + i) % m_numTaps;
			output += m_coeffs[i] * m_delayLine[index];
		}
		m_currentTap = (m_currentTap + 1) % m_numTaps;
		return output;
	}

private:
	int m_numTaps;
	std::vector<float> m_coeffs;
	std::vector<float> m_delayLine;
	int m_currentTap = 0;
};