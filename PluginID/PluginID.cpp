#include "mp_sdk_audio.h"

using namespace gmpi;

class PluginID final : public MpBase2
{
	// Unique ID for each instance
	uint64_t instanceId = {};

	// Static counter to generate unique IDs
	static uint64_t nextInstanceId;

	AudioInPin pinSignalin;
	IntOutPin pinIntID;
	IntOutPin pinInstanceID;


public:
	PluginID()
	{
		initializePin( pinSignalin );
		initializePin( pinIntID );
		initializePin(pinInstanceID);
		instanceId = ++nextInstanceId;
	}

	int32_t retrievePluginID(int value)
	{
		auto handle = getHost()->getHandle(value);
		pinIntID = value;
		pinInstanceID = instanceId;
		return handle; // 
	}

/*	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto signalin = getBuffer(pinSignalin);


		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.

			// Increment buffer pointers.
			++signalin;
			
		}
	}*/

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinSignalin.isStreaming() )
		{
		}

		retrievePluginID(static_cast<int32_t>(pinSignalin));

		// Set processing method.
		//setSubProcess(&PluginID::subProcess);
	}
};

uint64_t PluginID::nextInstanceId = 0;

namespace
{
	auto r = Register<PluginID>::withId(L"PluginID");
}
