// --- CMAKE generated variables for your plugin

#include "pluginstructures.h"

#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- AU Plugin Cocoa View Names (flat namespace) 
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_23815DFCE5E237729DED80254091E926
#define AU_COCOA_VIEW_NAME AUCocoaView_23815DFCE5E237729DED80254091E926

// --- BUNDLE IDs (MacOS Only) 
const char* kAAXBundleID = "developer.aax.jesefinal.bundleID";
const char* kAUBundleID = "developer.au.jesefinal.bundleID";
const char* kVST3BundleID = "developer.vst3.jesefinal.bundleID";

// --- Plugin Names 
const char* kPluginName = "JE-201";
const char* kShortPluginName = "JE-201";
const char* kAUBundleName = "JESEfinal_AU";
const char* kAAXBundleName = "JESEfinal_AAX";
const char* kVSTBundleName = "JESEfinal_VST";

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
const char* kVSTFUID = "{23815dfc-e5e2-3772-9ded-80254091e926}";

// --- 4-char codes 
const int32_t kFourCharCode = 'JESf';
const int32_t kAAXProductID = 'JESf';
const int32_t kManufacturerID = 'JEPl';

// --- Vendor information 
const char* kVendorName = "JEPlugins";
const char* kVendorURL = "";
const char* kVendorEmail = "";

// --- Plugin Options 
const bool kProcessFrames = true;
const uint32_t kBlockSize = DEFAULT_AUDIO_BLOCK_SIZE;
const bool kWantSidechain = false;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = aaxPlugInCategory_None;

#endif
