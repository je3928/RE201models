# RE201models

This is a (mostly) header only library implementing four seperate digital models of a Roland RE-201, developed throughout the duration of my final year project, on the BSc Sound Engineering and Production course at Birmingham City Univesity. The models are classes which can be implemented into an audio plugin in any framework.
Currently, the models function and have a tonality remeniscent of the RE-201. However, the models are currently very computationally expensive, particularly 2 and 4.
Future updates will increase optimisation by processing by block, multithreading or SIMD optimisations.


Credits:

Jatin Chowdhury - Tape magnetisation in models 2 and 4 implement a hysteresis processor based on the algorithm developed by chowdhury (2019)
https://github.com/jatinchowdhury18/AnalogTapeModel/tree/master/Plugin/Source/Processors/Hysteresis

Jatin Chowdhury - The tone stacks in models 3 and 4 implement WDF shelving filters which were contructed using the chowdsp_WDF library (2022)
https://github.com/Chowdhury-DSP/chowdsp_wdf

David T. Yeh - The tone stacks in models 1 and 2 were implemented using an algorithm based on the VA discretised 59 bassman tone stack.
https://ccrma.stanford.edu/~dtyeh/papers/yeh06_dafx.pdf

HI-FI LO-FI - All models implemented FFT convolution of the reverb only setting of a Boss RE-2. The real time convolution was implemented using HI-FI LO-FIs TwoStageFFTconvolver.
https://github.com/HiFi-LoFi/FFTConvolver

Will Pirkle - Plugins were prototyped using RackAFX and final plugins were generated using ASPIK.
https://github.com/willpirkleaudio/ASPiK

EarLevel Engineering - This biquad was used in tape speed dependant equalisation modelling.
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
