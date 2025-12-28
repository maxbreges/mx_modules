#include "mp_sdk_audio.h"

using namespace gmpi;

class Counter final : public MpBase2
{
	AudioInPin pinClock;
	AudioInPin pinReset;
	AudioInPin pinSteps;
	AudioOutPin pinCount;
	AudioInPin pinResetVal;

private:
	bool gate_state = false;
	bool reset_state = false;
	int counter = 0;
	int step_num = 0;
	int resetValue = 0;

public:
	Counter()
	{
		initializePin(pinClock);
		initializePin(pinReset);
		initializePin(pinSteps);
		initializePin(pinCount);
		initializePin(pinResetVal);
	}

	void subProcess(int sampleFrames)
	{
		// get pointers to in/output buffers.

		auto clock = getBuffer(pinClock);
		auto reset = getBuffer(pinReset);
		auto steps = getBuffer(pinSteps);
		auto count = getBuffer(pinCount);
		auto resetVal = getBuffer(pinResetVal);

		for (int s = sampleFrames; s > 0; s--)
		{
			step_num = *steps * 10.f;
			bool new_gate = *clock++ > 0.0f;
			if (new_gate != gate_state)
			{
				gate_state = new_gate;
				if (gate_state)
				{
					counter++;
					if (counter == step_num) {
						counter = 0;
					}
				}
			}
			steps++;
			*count++ = counter*0.1f;	
			resetValue = *resetVal++;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.


		pinClock.isUpdated();
		{
			
		}

		pinReset.isUpdated();
		{
			if ((pinReset) != reset_state)
			{
				reset_state = (pinReset);
				if (reset_state)
				{
					counter = resetValue*0.1;
				}
			}

		}
		// Set state of output audio pins.
		pinCount.setStreaming(true);
		// Set processing method.
		setSubProcess(&Counter::subProcess);
	}
};

namespace
{
	auto r = Register<Counter>::withId(L"Counter");
}
