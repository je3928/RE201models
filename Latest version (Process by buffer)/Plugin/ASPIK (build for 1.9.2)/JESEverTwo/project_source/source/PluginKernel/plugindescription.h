// --- CMAKE generated variables for your plugin

#include "pluginstructures.h"

#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- AU Plugin Cocoa View Names (flat namespace) 
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_7C863EADB3B63820A8FF9AD6E97D6A90
#define AU_COCOA_VIEW_NAME AUCocoaView_7C863EADB3B63820A8FF9AD6E97D6A90

// --- BUNDLE IDs (MacOS Only) 
const char* kAAXBundleID = "developer.aax.jesevertwo.bundleID";
const char* kAUBundleID = "developer.au.jesevertwo.bundleID";
const char* kVST3BundleID = "developer.vst3.jesevertwo.bundleID";

// --- Plugin Names 
const char* kPluginName = "JE-201v2";
const char* kShortPluginName = "JE-201v2";
const char* kAUBundleName = "JESEverTwo_AU";
const char* kAAXBundleName = "JESEverTwo_AAX";
const char* kVSTBundleName = "JESEverTwo_VST";

// --- bundle name helper 
inline static const char* getPluginDescBundleName() 
{ 
#ifdef AUPLUGIN 
	return kAUBundleName; 
#endif 

#ifdef AAXPLUGIN 
	return kAAXBundleName; 
#endif 

#ifdef VSTPLUGIN 
	return kVSTBundleName; 
#endif 

	// --- should never get here 
	return kPluginName; 
} 

// --- Plugin Type 
const pluginType kPluginType = pluginType::kFXPlugin;

// --- VST3 UUID 
const char* kVSTFUID = "{7c863ead-b3b6-3820-a8ff-9ad6e97d6a90}";

// --- 4-char codes 
const int32_t kFourCharCode = 'JEVt';
const int32_t kAAXProductID = 'JEVt';
const int32_t kManufacturerID = 'JEPl';

// --- Vendor information 
const char* kVendorName = "JEPlugins";
const char* kVendorURL = "www.yourcompany.com";
const char* kVendorEmail = "help@yourcompany.com";

// --- Plugin Options 
const bool kProcessFrames = false;
const uint32_t kBlockSize = 0;
const bool kWantSidechain = false;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = aaxPlugInCategory_None;

#endif
