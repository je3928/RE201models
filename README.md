# RE201models

This repository is dedicated to digital modelling of the Roland RE-201. 

![image](https://github.com/user-attachments/assets/af2c5b00-7f3b-427c-9058-f0eb94b3c5e8)

This was my final year project on the BSc Sound Engineering and Production course at Birmingham City University. The DSP code implements two variations of tone stack emulation and two variations of tape saturation emulation. Other effects modelled include control ballistics, wow and flutter, tape speed dependant equalisation and spring reverberation.

The project is currently packaged as a JUCE plugin, by default the plugin implements a WDF shelving tone stack and physical tape hysteresis processing. The tone stack object can be toggled to change from WDF to a VA tone stack. The tape magnetisation object can be toggled from tape hysteresis processing to a more traditional error function based sigmoid non-linearity. Reverberation is implemented by FFT convolution of an impulse reponse of the reverb only setting of a Boss RE-2. Since leaving university I have added a waveguide spring reverb algorithm and the plugin allows toggling between FFT reverb and waveguide. Future work is planned to improve the waveguide algorithm as it currently sounds quite robotic.  

Credits:

Jatin Chowdhury - Tape magnetisation implemented using a hysteresis processor based on the algorithm developed by Chowdhury (2019)
https://github.com/jatinchowdhury18/AnalogTapeModel/

Jatin Chowdhury - WDF tone stack implemented using WDF shelving filters which were contructed using the chowdsp_WDF library (2022)
https://github.com/Chowdhury-DSP/chowdsp_wdf

S.A. Samad (2003) - This paper details the theory behind parametric and shelving WDF bands.
https://journals.tubitak.gov.tr/elektrik/vol11/iss3/3/

David T. Yeh - VA tone stack implemented using an algorithm based on the VA discretised 59 bassman tone stack.
https://ccrma.stanford.edu/~dtyeh/papers/yeh06_dafx.pdf

HI-FI LO-FI - All models implemented FFT convolution of the reverb only setting of a Boss RE-2. The real time convolution was implemented using HI-FI LO-FIs TwoStageFFTconvolver.
https://github.com/HiFi-LoFi/FFTConvolver

Will Pirkle - Plugins were prototyped using RackAFX and original plugins were generated using ASPIK.
https://github.com/willpirkleaudio/ASPiK

EarLevel Engineering - This biquad was used in tape speed dependant equalisation modelling, modified to be direct form 2 transposed.
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/

Foleys GUI Magic - GUI for JUCE version of the plugin was created using foleys GUI magic.
https://github.com/ffAudio/foleys_gui_magic



