#include "mp_sdk_gui2.h"
#include <sstream>

using namespace gmpi;
using namespace std;

class IntToHexGui final : public SeGuiInvisibleBase
{
 	void onSetChoice()
	{
		std::stringstream ss;
		ss << std::hex << pinChoice; // int decimal_value
		std::string res(ss.str());

		pinItemText = res;
	}

 	void onSetItemText()
	{
		// pinItemText changed
	}

 	IntGuiPin pinChoice;
 	StringGuiPin pinItemText;

public:
	IntToHexGui()
	{
		initializePin( pinChoice, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetChoice) );
		initializePin( pinItemText, static_cast<MpGuiBaseMemberPtr2>(&IntToHexGui::onSetItemText) );
	}

};

namespace
{
	auto r = Register<IntToHexGui>::withId(L"Int to Hex");
}
