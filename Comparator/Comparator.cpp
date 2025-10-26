#include "mp_sdk_audio.h"

using namespace gmpi;

class Comparator final : public MpBase2
{
	//float lastOutputValue = 0;

	AudioInPin pinInputA;
	AudioInPin pinInputB;
	AudioOutPin pinOutput;
	FloatInPin pinHiOutVal;
	FloatInPin pinEqOutVal;
	FloatInPin pinLoOutVal;

public:
	Comparator()
	{
		initializePin( pinInputA );
		initializePin( pinInputB );
		initializePin( pinOutput );
		initializePin( pinHiOutVal );
		initializePin(pinEqOutVal);
		initializePin( pinLoOutVal );
	}

	void subProcess( int sampleFrames )
	{
		// get pointers to in/output buffers.
		auto inputA = getBuffer(pinInputA);
		auto inputB = getBuffer(pinInputB);
		auto output = getBuffer(pinOutput);

		for( int s = sampleFrames; s > 0; --s )
		{
			// TODO: Signal processing goes here.
			if (*inputA > *inputB)
			{
				*output = pinHiOutVal*0.1;
				//lastOutputValue = *output;
			}
			if (*inputA < *inputB)
			{
				*output = pinLoOutVal*0.1;
				//lastOutputValue = *output;
			}
			if (*inputA == *inputB)
			{
				*output = pinEqOutVal*0.1;
			}
			// Increment buffer pointers.
			++inputA;
			++inputB;
			++output;
		}
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinInputA.isStreaming() )
		{
		}
		if( pinInputB.isStreaming() )
		{
		}
		if( pinHiOutVal.isUpdated() )
		{
		}
		if (pinEqOutVal.isUpdated())
		{
		}
		if( pinLoOutVal.isUpdated() )
		{
		}

		// Set state of output audio pins.
		pinOutput.setStreaming(true);

		// Set processing method.
		setSubProcess(&Comparator::subProcess);
	}
};

namespace
{
	auto r = Register<Comparator>::withId(L"ComparatorEq");
}
