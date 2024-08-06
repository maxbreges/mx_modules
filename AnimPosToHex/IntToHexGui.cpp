#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;
using namespace std;

class IntToHexGui final : public SeGuiInvisibleBase
{
 	void onSetAnimPos()
	{
		float x = pinAnimPos;
		float _hex = x * 255.f;
		pinIntOut = _hex;

		std::stringstream ss;
		ss << std::hex << pinIntOut; // int decimal_value
		std::string res(ss.str());

		pinItemText = res;
	}

 	void onSetItemText()
	{	
	}
	void onSetIntOut()
	{
	}

 	FloatGuiPin pinAnimPos;
 	StringGuiPin pinItemText;
	IntGuiPin pinIntOut;

public:
	IntToHexGui()
	{
		initializePin( pinAnimPos, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetAnimPos) );
		initializePin( pinItemText, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetItemText) );
		initializePin(pinIntOut, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetIntOut));
	}

};

namespace
{
	auto r = Register<IntToHexGui>::withId(L"AnimPos to Hex");
}
