// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
    \file   plugincore.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  base class interface file for ASPiK plugincore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"
#include "RE201modelNew/RE201model.h"

// **--0x7F1F--**

// --- Plugin Variables controlID Enumeration 

enum controlID {
	bass = 0,
	treble = 1,
	repeatRate = 2,
	intensity = 3,
	delaySetting = 6,
	reverbvolume = 4,
	echovolume = 5,
	m_fMeterValue = 7,
	inputLevel = 8,
	WetDry = 9,
	os_amount = 10
};

	// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)


PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
    PluginCore();

	/** Destructor: empty in default version */
    virtual ~PluginCore(){}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);

	/** preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding */
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data (DEFAULT MODE) */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	/** Pre-process the block with: MIDI events for the block and parametet smoothing */
	virtual bool preProcessAudioBlock(IMidiEventQueue* midiEventQueue = nullptr);

	/** process sub-blocks of data (OPTIONAL MODE) */
	virtual bool processAudioBlock(ProcessBlockInfo& processBlockInfo);

	/** This is the native buffer processing function; you may override and implement
	     it if you want to roll your own buffer or block procssing code */
	// virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- example block processing template functions (OPTIONAL)
	//
	/** FX EXAMPLE: process audio by passing through */
	bool renderFXPassThrough(ProcessBlockInfo& blockInfo);

	/** SYNTH EXAMPLE: render a block of silence */
	bool renderSynthSilence(ProcessBlockInfo& blockInfo);

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here

	std::unique_ptr<RE201model> tapemodel = nullptr;

    // Plugin info
	int NumChannels = 2;
	float SampleRate = 48000;

    // Current multi tap playhead state
	int playheadstates[3] = { 0, 0, 0 };

    // Arrays used to enable reverb or delay
	int reverbEnabled[12] = { 0, 0 ,0 ,0 , 1, 1, 1, 1, 1, 1, 1, 1 };
	int delayEnabled[12] = { 1, 1 ,1 ,1, 1, 1, 1, 1, 1, 1, 1, 0 };
    
    // OS amounts
	int OSamount[5] = { 2, 4, 6, 8, 16};


    // Function to apply playhead states based on mode selection
	void applyDelaySettings() {

		if (compareEnumToInt(delaySettingEnum::One, delaySetting)) {

			playheadstates[0] = 1;
			playheadstates[1] = 0;
			playheadstates[2] = 0;


		}

		if (compareEnumToInt(delaySettingEnum::Two, delaySetting)) {

			playheadstates[0] = 0;
			playheadstates[1] = 1;
			playheadstates[2] = 0;


		}

		if (compareEnumToInt(delaySettingEnum::Three, delaySetting)) {

			playheadstates[0] = 0;
			playheadstates[1] = 0;
			playheadstates[2] = 1;


		}

		if (compareEnumToInt(delaySettingEnum::Four, delaySetting)) {

			playheadstates[0] = 0;
			playheadstates[1] = 1;
			playheadstates[2] = 1;


		}

		if (compareEnumToInt(delaySettingEnum::Five, delaySetting)) {

			playheadstates[0] = 1;
			playheadstates[1] = 0;
			playheadstates[2] = 0;


		}

		if (compareEnumToInt(delaySettingEnum::Six, delaySetting)) {

			playheadstates[0] = 0;
			playheadstates[1] = 1;
			playheadstates[2] = 0;


		}

		if (compareEnumToInt(delaySettingEnum::Seven, delaySetting)) {

			playheadstates[0] = 0;
			playheadstates[1] = 0;
			playheadstates[2] = 1;


		}

		if (compareEnumToInt(delaySettingEnum::Eight, delaySetting)) {

			playheadstates[0] = 1;
			playheadstates[1] = 1;
			playheadstates[2] = 0;


		}

		if (compareEnumToInt(delaySettingEnum::Nine, delaySetting)) {

			playheadstates[0] = 0;
			playheadstates[1] = 1;
			playheadstates[2] = 1;


		}

		if (compareEnumToInt(delaySettingEnum::Ten, delaySetting)) {

			playheadstates[0] = 1;
			playheadstates[1] = 0;
			playheadstates[2] = 1;


		}

		if (compareEnumToInt(delaySettingEnum::Eleven, delaySetting)) {

			playheadstates[0] = 1;
			playheadstates[1] = 1;
			playheadstates[2] = 1;


		}

	}

	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

protected:

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	float bass = 0.f;
	float treble = 0.f;
	float repeatRate = 0.f;
	float intensity = 0.f;
	float reverbvolume = 0.f;
	float echovolume = 0.f;
	float inputLevel = 0.f;
	float WetDry = 0.f;

	// --- Discrete Plugin Variables 
	int delaySetting = 0;
	enum class delaySettingEnum { One,Two,Three,Four,Five,Six,Seven,Eight,Nine,Ten,Eleven,Reverb };	// to compare: if(compareEnumToInt(delaySettingEnum::One, delaySetting)) etc... 

	int os_amount = 0;
	enum class os_amountEnum { One,Two,Four,Eight,Sixteen };	// to compare: if(compareEnumToInt(os_amountEnum::One, os_amount)) etc... 

	// --- Meter Plugin Variables
	float m_fMeterValue = 0.f;

	// **--0x1A7F--**
    // --- end member variables

public:
    /** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
    static const char* getPluginBundleName();

    /** static description: name

	\return name as a const char*
	*/
    static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();

    /** Status Window Messages for hosts that can show it */
    void sendHostTextMessage(std::string messageString)
    {
        HostMessageInfo hostMessageInfo;
        hostMessageInfo.hostMessage = sendRAFXStatusWndText;
        hostMessageInfo.rafxStatusWndText.assign(messageString);
        if(pluginHostConnector)
            pluginHostConnector->sendHostMessage(hostMessageInfo);
    }

};




#endif /* defined(__pluginCore_h__) */