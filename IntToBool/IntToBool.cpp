#include "mp_sdk_audio.h"

using namespace gmpi;

class IntToBool final : public MpBase2
{
	IntInPin pinIntVal;
	BoolOutPin pinBoolVal;

public:
	IntToBool()
	{
		initializePin( pinIntVal );
		initializePin( pinBoolVal );
	}

	void onSetPins() override
	{
		// Check which pins are updated.
		if( pinIntVal.isUpdated() )
		{
			if (pinIntVal)
			{
				pinBoolVal = true;
			}
			if (pinIntVal <= 0)
			{
				pinBoolVal = false;
			}
		}
	}
};

namespace
{
	auto r = Register<IntToBool>::withId(L"Int to Bool");
}
