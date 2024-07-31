#include "mp_sdk_gui2.h"
#include "mp_midi.h"

//SE_DECLARE_INIT_STATIC_FILE(NoteOffToBool);

using namespace gmpi;

class NoteOffToBoolGui final : public SeGuiInvisibleBase
{
	void onSetBoolIn()
	{
		pinBoolOut = pinBoolIn;
	}
	
private:
	BoolGuiPin pinBoolIn;

public:
	BoolGuiPin pinBoolOut;


	NoteOffToBoolGui()
	{
		initializePin(pinBoolIn, static_cast<MpGuiBaseMemberPtr2>(&NoteOffToBoolGui::onSetBoolIn));
		initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&NoteOffToBoolGui::onSetBoolIn));
	}

};

		namespace
		{
			auto r = Register<NoteOffToBoolGui>::withId(L"NoteOffToBool");
		}
