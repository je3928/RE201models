#pragma once

class WetDryProcessor {
public:
    WetDryProcessor() {}

    void reset(int numChannels) 
    {
        NumChannels = numChannels;


    }


    //output is written to wet buffer
    void ProcessBuffer(std::vector<std::vector<float>>& dryBuffer, std::vector<std::vector<float>>& wetBuffer, int blockSize, float Wet, float outputBufferScale)
    {
        wet = Wet;

        BlockSize = blockSize;

        float dry = 1.0f - wet;

        for (int channel = 0; channel < NumChannels; channel++)
        {
            for (int sample = 0; sample < BlockSize; sample++)
            {
                wetBuffer[channel][sample] = (dry * dryBuffer[channel][sample] + wet * wetBuffer[channel][sample]) * outputBufferScale;
            }
        }
    }

private:
    int NumChannels = 2;
    int BlockSize = 512;

    float wet = 0.0f;
};
