#include "mp_sdk_gui2.h"

using namespace gmpi;

class GuiORGui final : public SeGuiInvisibleBase
{
 	void onSetOut()
	{
		// pinChoice changed
	}

 	void onSetIn()
	{
		if ((pin1 || pin2 || pin3 || pin4 || pin5 || pin6 || pin7 || pin8 || pin9 || pin10 || pin11 || pin12 || pin13 || pin14 || pin15 || pin16 || pin17 || pin18 || pin19 || pin20) != 0)

			pinOutR = true;
			pinOut = pinOutR;

		if ((pin1 || pin2 || pin3 || pin4 || pin5 || pin6 || pin7 || pin8 || pin9 || pin10 || pin11 || pin12 || pin13 || pin14 || pin15 || pin16 || pin17 || pin18 || pin19 || pin20) == 0)
			
			pinOutR = false;
			pinOut = pinOutR;

	}
	BoolGuiPin pinOutR;
 	BoolGuiPin pinOut;
 	BoolGuiPin pin1;
 	BoolGuiPin pin2;
	BoolGuiPin pin3;
	BoolGuiPin pin4;
	BoolGuiPin pin5;
	BoolGuiPin pin6;
	BoolGuiPin pin7;
	BoolGuiPin pin8;
	BoolGuiPin pin9;
	BoolGuiPin pin10;
	BoolGuiPin pin11;
	BoolGuiPin pin12;
	BoolGuiPin pin13;
	BoolGuiPin pin14;
	BoolGuiPin pin15;
	BoolGuiPin pin16;
	BoolGuiPin pin17;
	BoolGuiPin pin18;
	BoolGuiPin pin19;
	BoolGuiPin pin20;

public:
	GuiORGui()
	{
		initializePin(pinOutR, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetOut));
		initializePin( pinOut, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetOut) );
		initializePin( pin1, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn) );
		initializePin( pin2, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn) );
		initializePin(pin3, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin4, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin5, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin6, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin7, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin8, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin9, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin10, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin11, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin12, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin13, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin14, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin15, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin16, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin17, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin18, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin19, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));
		initializePin(pin20, static_cast<MpGuiBaseMemberPtr2>(&GuiORGui::onSetIn));

	}

};

namespace
{
	auto r = Register<GuiORGui>::withId(L"MX GuiOR");
}
