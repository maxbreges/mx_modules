#include "mp_sdk_audio.h"

using namespace gmpi;

class SampleHold final : public MpBase2
{
	AudioInPin pinAudio;
	AudioInPin pinHold;
	AudioOutPin pinOutput;
	FloatOutPin pinParamOut;

private:
	bool gate_state = false;
	float hold_audio = 0;

public:
	SampleHold()
	{
		initializePin(pinAudio);
		initializePin(pinHold);
		initializePin(pinOutput);
		initializePin(pinParamOut);
	}

	void subProcess(int sampleFrames)
	{
		// get pointers to in/output buffers.
		auto audio = getBuffer(pinAudio);
		auto hold = getBuffer(pinHold);
		auto output = getBuffer(pinOutput);

		for (int s = sampleFrames; s > 0; s--)
		{
			bool new_gate = *hold++ > 0.0f;
			if (new_gate != gate_state)
			{
				gate_state = new_gate;
				if (gate_state)
				{
					hold_audio = *audio;

				}
			}

			*output++ = hold_audio;
			pinParamOut.setValue(hold_audio, getBlockPosition());
			audio++;
		}
	}

	int32_t receiveMessageFromGui(int32_t id, int32_t size, const void* msg)
	{
		if (id != 119 || size != sizeof(float) || msg == nullptr)
			return gmpi::MP_FAIL; // Or ignore invalid messages per your SDK.

		std::memcpy(&hold_audio, msg, sizeof(hold_audio));
		return gmpi::MP_OK;
	}

	void onSetPins() override
	{
		getHost()->sendMessageToGui(120, 0, nullptr);

		if (pinParamOut.isUpdated())
		{
			//hold_audio = pinParamOut;
		}

		// Check which pins are updated.
		if (pinAudio.isStreaming())
		{
		}
		if (pinHold.isStreaming())
		{
		}

		// Set state of output audio pins.
		pinOutput.setStreaming(true);
		// Set processing method.
		setSubProcess(&SampleHold::subProcess);
	}
};

namespace
{
	auto r = Register<SampleHold>::withId(L"SampleHold");
}
