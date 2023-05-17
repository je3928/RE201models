# RE201models

This repository is dedicated to digital modelling of the Roland RE-201. 

The original folder contains a (mostly) header only library implementing four seperate digital models of a Roland RE-201, developed throughout the duration of my final year project, on the BSc Sound Engineering and Production course at Birmingham City Univesity. The models are classes which can be implemented into an audio plugin in any framework. 

The four models implemented combinations of two tape saturation and two tone stack modellings methods. One tape saturation modelling method was a static non linearity (error function) and the other was based on the hysteresis processor developed by chowdhury (2019) which calculates magnetisation based on a tape playhead position derivative approximation. The tape delay modelling between the two methods is the same, utilising three circular buffers with read times at multiple of each other, in line with the real device (Using a single buffer with multiple read heads was tested, however this appeared to generate phasing issues when multiple playheads were active). Both methods implement oversampling, albeit very inefficiently. Version 2 attempts to resolve many of those issues. 

The first tone stack method implemented the VA Bassman 59' filter discretised by Yeh (2006). The other tone stack was two WDF shelving filters processed in series to make up the bass and treble frequency bands. 

Other perceptual characteristics have been modelled, this includes wow and flutter (although this is next on my improvements list), control ballistics (pitch increases when the repeat rate is increasing and vice versa) and tape speed dependant equalisation. 

Version 2 is a significant improvement of the original. This is dedicated to optimisations which did not fit into the time scale of my bachelors project and generally cleans up the code. This includes processing by buffer instead of sample, optimising the filter structures, adjustments to the WDF tone stack, reducing unnecessary calculations and dynamic allocation of objects and memory based on channel count. The error function tape saturation model has been omitted (because lets be honest, tape hysteresis modelling is way cooler), however the tone stack algorithm can be switched between VA and WDF by setting a bool variable in the RE201model class.


Included in both versions is the respective Windows VST x64 plugin and ASPIK project which can be built for Mac OS. Mac OS versions will be added shortly.


Credits:

Jatin Chowdhury - Tape magnetisation in models 2 and 4 implement a hysteresis processor based on the algorithm developed by Chowdhury (2019)
https://github.com/jatinchowdhury18/AnalogTapeModel/

Jatin Chowdhury - The tone stacks in models 3 and 4 implement WDF shelving filters which were contructed using the chowdsp_WDF library (2022)
https://github.com/Chowdhury-DSP/chowdsp_wdf

S.A. Samad (2003) - This paper details the theory behind parametric and shelving WDF bands.
https://journals.tubitak.gov.tr/elektrik/vol11/iss3/3/

David T. Yeh - The tone stacks in models 1 and 2 were implemented using an algorithm based on the VA discretised 59 bassman tone stack.
https://ccrma.stanford.edu/~dtyeh/papers/yeh06_dafx.pdf

HI-FI LO-FI - All models implemented FFT convolution of the reverb only setting of a Boss RE-2. The real time convolution was implemented using HI-FI LO-FIs TwoStageFFTconvolver.
https://github.com/HiFi-LoFi/FFTConvolver

Will Pirkle - Plugins were prototyped using RackAFX and final plugins were generated using ASPIK.
https://github.com/willpirkleaudio/ASPiK

EarLevel Engineering - This biquad was used in tape speed dependant equalisation modelling, modified to be direct form 2 transposed.
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
