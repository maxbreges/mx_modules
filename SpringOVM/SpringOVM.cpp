#include "mp_sdk_gui2.h"

using namespace gmpi;

class SpringOVMGui final : public SeGuiInvisibleBase
{
	bool prevMouseDown = false;

 	void onSetAnimationPosition()
	{
		// pinAnimationPosition changed
	}

 	void onSetMouseDown()
	{
		// mouseDown changed
		if (pinOnOff && pinMouseDown == false && prevMouseDown == true)
		{
			pinAnimationPosition = pinResetValue;
		}

		prevMouseDown = pinMouseDown;
	}

 	void onSetMouseDownOut()
	{
		if (pinOnOff && pinMouseDown == false && prevMouseDown == true)
		{
			pinAnimationPosition = pinResetValue;
		}

		prevMouseDown = pinMouseDownOut;
	}

 	void onSetResetValue()
	{
		// pinResetValue changed
	}

 	void onSetOnOff()
	{
		// pinOnOff changed
	}

 	FloatGuiPin pinAnimationPosition;
 	BoolGuiPin pinMouseDown;
 	BoolGuiPin pinMouseDownOut;
 	FloatGuiPin pinResetValue;
 	BoolGuiPin pinOnOff;

public:
	SpringOVMGui()
	{
		initializePin( pinAnimationPosition, static_cast<MpGuiBaseMemberPtr2>(&SpringOVMGui::onSetAnimationPosition) );
		initializePin( pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&SpringOVMGui::onSetMouseDown) );
		initializePin( pinMouseDownOut, static_cast<MpGuiBaseMemberPtr2>(&SpringOVMGui::onSetMouseDownOut) );
		initializePin( pinResetValue, static_cast<MpGuiBaseMemberPtr2>(&SpringOVMGui::onSetResetValue) );
		initializePin( pinOnOff, static_cast<MpGuiBaseMemberPtr2>(&SpringOVMGui::onSetOnOff) );
	}

};

namespace
{
	auto r = Register<SpringOVMGui>::withId(L"SpringOVM");
}
