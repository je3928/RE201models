#include "WaveguideSpringReverb.h"

WaveguideUnit::WaveguideUnit()
{

}

WaveguideUnit::~WaveguideUnit()
{

}

float WaveguideUnit::ProcessSample(float xn)
{
	delayBuffer.BufferWrite(xn);

	float bufferout = delayBuffer.BufferRead(delayTimeSeconds * 1000.f, true);

	float yn = dispertion1.ProcessSample(bufferout);
	yn = dispertion2.ProcessSample(yn);
	yn = dispertion3.ProcessSample(yn);
	yn = dispertion4.ProcessSample(yn);
	yn = dispertion5.ProcessSample(yn);
	yn = OutputFilter1.ProcessSample(yn);
	yn = OutputFilter2.ProcessSample(yn);

	return yn * attenuation;

}

void WaveguideUnit::Reset(float sampleRate, float HPF_FC, float LPF_FC)
{
	delayBuffer.Init(sampleRate, 500.f);
	dispertion1.Init(sampleRate);
	dispertion2.Init(sampleRate);
	dispertion3.Init(sampleRate);
	dispertion4.Init(sampleRate);
	dispertion5.Init(sampleRate);
	OutputFilter1.Init(LPF, LPF_FC, sampleRate, 0.707, 0.f);
	OutputFilter2.Init(HPF, HPF_FC, sampleRate, 0.707, 0.f);


}

void WaveguideUnit::SetParameters(float RT60, float DelayTimeSeconds)
{
	delayTimeSeconds = DelayTimeSeconds;
	rt60 = RT60;

}
