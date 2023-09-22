/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JE201AudioProcessor::JE201AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
: foleys::MagicProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),treestate(*this, nullptr, "parameters", params())
#endif
{
    
    FOLEYS_SET_SOURCE_PATH(__FILE__);
    
    magicState.setGuiValueTree(BinaryData::JE201GuiNew3_xml, BinaryData::JE201GuiNew3_xmlSize);
    
    // Set this processor object as a listener to the value tree
    treestate.state.addListener(this);

    // Add parameters to treestate
    populateParameters();

    // Clear atomic flag so that parameters are saved to internal variables
    changesApplied.clear();
}

JE201AudioProcessor::~JE201AudioProcessor()
{
}

//==============================================================================
const juce::String JE201AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JE201AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JE201AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JE201AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JE201AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JE201AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JE201AudioProcessor::getCurrentProgram()
{
    return 0;
}

void JE201AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JE201AudioProcessor::getProgramName (int index)
{
    return {};
}

void JE201AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JE201AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Clear atomic flag so that parameters are saved to internal variables
    changesApplied.clear();
    
    // Reset tape model object, Set OS Amount to 1 (disabled for now, fix in future updates), set max channel count to 2.
    echomodel.Reset(sampleRate, 1, 2);
    
}

void JE201AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JE201AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JE201AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update dsp object with plugin parameter values
    updateParameters();
  
    // Get write pointer to channels
    auto bufferpointer = buffer.getArrayOfWritePointers();


    // Initialise processing buffer, applying buffer to vectorvectorfloat array allows easier duplication if parallel processing is required
    std::vector<std::vector<float>> processingbuffer;
 
    // Resize processing buffer
    processingbuffer.resize(totalNumOutputChannels);
    for (int i = 0; i < totalNumOutputChannels; ++i)
    {
        processingbuffer[i].resize(buffer.getNumSamples());

    }
    
    // Apply audio to processing buffer
    for (int channel = 0; channel < totalNumOutputChannels; channel++)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            // if mono stereo apply only input channel to both output channels
            if (totalNumInputChannels == 1)
                processingbuffer[channel][sample] = bufferpointer[0][sample];
            else
                processingbuffer[channel][sample] = bufferpointer[channel][sample];
        }
    }

    // Apply signal processing
    echomodel.ProcessBuffer(processingbuffer, buffer.getNumSamples());
    
    // Apply processed audio to buffer write pointer
    for (int channel = 0; channel < totalNumOutputChannels; channel++)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            bufferpointer[channel][sample] = processingbuffer[channel][sample];
        }
    }
    
    
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JE201AudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout JE201AudioProcessor::params() {

    juce::AudioProcessorValueTreeState::ParameterLayout parameters;

    auto inputgroup = std::make_unique<juce::AudioProcessorParameterGroup>("Input", "INPUT", "|");

    inputgroup->addChild(std::make_unique<AudioParameterFloat>("InputLevel", "INPUTLEVEL", 0.0f, 5.0f, 0.5f));
    inputgroup->addChild(std::make_unique<AudioParameterFloat>("WetDry", "WETDRY", 0.0f, 1.0f, 0.5f));
    inputgroup->addChild(std::make_unique<AudioParameterFloat>("Bass", "BASS", 0.0f, 1.0f, 0.5f));
    inputgroup->addChild(std::make_unique<AudioParameterFloat>("Treble", "TREBLE", 0.0f, 1.0f, 0.5f));

    parameters.add(std::move(inputgroup));


    auto delaygroup = std::make_unique<juce::AudioProcessorParameterGroup>("Delay", "DELAY", "|");

    delaygroup->addChild(std::make_unique<AudioParameterChoice>("DelaySetting", "DELAYSETTING", StringArray("1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "Reverb Only"), 0));
    delaygroup->addChild(std::make_unique<AudioParameterFloat>("RepeatRate", "REPEATRATE", 0.0f, 1.0f, 0.5f));
    delaygroup->addChild(std::make_unique<AudioParameterFloat>("Intensity", "INTENSITY", 0.0f, 1.0f, 0.5f));

    parameters.add(std::move(delaygroup));

    auto mastergroup = std::make_unique<juce::AudioProcessorParameterGroup>("Master", "MASTER", "|");

    mastergroup->addChild(std::make_unique<AudioParameterChoice>("ReverbType", "REVERBTYPE", StringArray("Convolution", "Waveguide"), 0));
    //mastergroup->addChild(std::make_unique<AudioParameterChoice>("OsAmount", "OSAMOUNT", StringArray("1", "2", "4", "8", "16"), 0));
    mastergroup->addChild(std::make_unique<AudioParameterFloat>("ReverbVolume", "REVERBVOLUME", 0.0f, 1.0f, 0.5f));
    mastergroup->addChild(std::make_unique<AudioParameterFloat>("EchoVolume", "ECHOVOLUME", 0.0f, 1.0f, 0.5f));

    parameters.add(std::move(mastergroup));

    return parameters;

}



void JE201AudioProcessor::populateParameters() {

    

    InputLevel = treestate.getRawParameterValue("InputLevel");
    WetDry = treestate.getRawParameterValue("WetDry");
    Bass = treestate.getRawParameterValue("Bass");
    Treble = treestate.getRawParameterValue("Treble");
    
    
    DelaySetting = (static_cast<AudioParameterChoice*>(treestate.getParameter("DelaySetting")));
    RepeatRate = treestate.getRawParameterValue("RepeatRate");
    Intensity = treestate.getRawParameterValue("Intensity");

    ReverbType = (static_cast<AudioParameterChoice*>(treestate.getParameter("ReverbType")));
    //OSAmount = (static_cast<AudioParameterChoice*>(treestate.getParameter("OsAmount")));
    ReverbVolume = treestate.getRawParameterValue("ReverbVolume");
    EchoVolume = treestate.getRawParameterValue("EchoVolume");

}

void JE201AudioProcessor::updateParameters() {

    if (changesApplied.test_and_set())
    {
        return;
    }
    
    applyDelaySettings();
    echomodel.UpdateParameters(*Bass, *Treble, *Intensity, *RepeatRate, playheadstates, delayEnabled[*DelaySetting], *EchoVolume, reverbEnabled[*DelaySetting], *ReverbVolume, *InputLevel, *WetDry, *ReverbType);
    


}

void JE201AudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged,
    const juce::Identifier& property)
{
    // Clear atomic flag so that parameters are saved to internal variables
    changesApplied.clear();
}
