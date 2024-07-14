#include "mp_sdk_audio.h"

using namespace gmpi;

class CounterX final : public MpBase2
{
	
	AudioInPin pinClock;
	AudioInPin pinReset;
	AudioInPin pinSteps;
	AudioOutPin pinCount;

	bool trig_state = false;
	bool reset_state = false;
	int counter = 0;
	int step_num = 0;

public:
	CounterX()
	{
		
		initializePin( pinClock );
		initializePin( pinReset );
		initializePin( pinSteps );
		initializePin( pinCount );		
	
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto clock = getBuffer(pinClock);
		auto reset = getBuffer(pinReset);
		auto steps = getBuffer(pinSteps);
		auto count = getBuffer(pinCount);
				

		while (sampleFrames --)
		{
			// TODO: Signal processing goes here.
			*count = (float)counter * 0.1f;
			step_num = *steps * 10.f;

			// Increment buffer pointers.
			++clock;
			++reset;
			++steps;
			++count;
						
		}
				
	}

	
	void onSetPins() override
	{
		// Check which pins are updated.
		pinReset.isUpdated();
		if ((pinReset>0) != reset_state)
		{
			reset_state = (pinReset);
			if (reset_state)
			{
				counter = 0;
			}
		}

		pinClock.isUpdated();
		if ((pinClock>0) != trig_state)
		{
			trig_state = (pinClock);
			if (trig_state) 
			{
				counter++;
				if (counter == step_num) {
					counter = 0;
				}
			}
				
		}
		

		// Set state of output audio pins.
		pinCount.setStreaming(true);

		// Set processing method.
		setSubProcess(&CounterX::subProcess);
	}
};

namespace
{
	auto r = Register<CounterX>::withId(L"mx Counter");
}
