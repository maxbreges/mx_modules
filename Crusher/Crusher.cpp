#include "FilterBase.h"
#include "xp_simd.h"

using namespace gmpi;

class Crusher final : public FilterBase
{
	AudioInPin pinSignalIn;
	AudioInPin pinDownsample;
	AudioOutPin pinSignalOut;
	float downsampleCount_ = {};
	float held_ = {};

public:
	Crusher()
	{
		initializePin( pinSignalIn );
		initializePin( pinDownsample );
		initializePin( pinSignalOut );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto signalIn = getBuffer(pinSignalIn);
		auto downsample = getBuffer(pinDownsample);
		auto signalOut = getBuffer(pinSignalOut);

		constexpr auto c = 60.f / 200000.0f;
		const float downsampleRatio = (std::max)(1.0f, 1.0f + (*downsample * *downsample) * getSampleRate() * c);

		for( int s = sampleFrames; s > 0; --s )
		{
			downsampleCount_ -= 1.0f;
			if (downsampleCount_ <= 0.0f)
			{
				*signalOut = held_ = *signalIn;

				downsampleCount_ += downsampleRatio;
			}
			else
			{
				*signalOut = held_;
			}

			++signalIn;
			++signalOut;
		}
	}

	void onSetPins(void) override
	{
		// Set state of output audio pins.
		pinSignalOut.setStreaming(true);

		// Set processing method.
		setSubProcess(&Crusher::subProcess);

		initSettling(); // must be last.
	}

	bool isFilterSettling() override
	{
		return !pinSignalIn.isStreaming() && !pinDownsample.isStreaming();
	}
	AudioOutPin& getOutputPin() override
	{
		return pinSignalOut;
	}
};

namespace
{
	auto r = Register<Crusher>::withId(L"Example Crusher");
}
