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
        // Set channels to buffer size if inconsistent,
        // avoids segmentation errors when mono only.
        if (dryBuffer.size() != NumChannels)
            NumChannels = dryBuffer.size();
        
        // Save wet variable
        wet = Wet;

        // Save block size
        BlockSize = blockSize;

        // calculate dry based on wet value
        float dry = 1.0f - wet;

        // Apply mix
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
