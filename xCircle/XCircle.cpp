#include "mp_sdk_audio.h"
#include "mp_midi.h"

using namespace gmpi;

class XCircle final : public MpBase2

{
private:
	BoolInPin pinFromGui;
	BoolOutPin pinToGui;

public:
	BoolInPin pinGateIn;
	MidiOutPin pinMIDIOut;
	IntInPin pinChan;
	IntInPin pinNote;

	bool trig_state = false;

	XCircle()
	{
		initializePin(pinFromGui);
		initializePin(pinToGui);
		initializePin(pinGateIn);
		initializePin(pinMIDIOut);		
		initializePin(pinChan);
		initializePin(pinNote);
	}

	void onSetPins(void)  // one or more pins_ updated.  Check pin update flags to determin which ones.
	{
		if (pinGateIn.isUpdated())
		{

			if ((pinGateIn) != trig_state)
			{
				trig_state = pinGateIn;
				if (pinGateIn)
				{
					pinToGui = true;
				}
				if (!pinGateIn)
				{
					pinToGui = false;
				}
			}
		}

		unsigned char midiMessage[3] = { 0,0,0 };
		unsigned char chan = 0;
		unsigned char velocity = 100;

		if (pinFromGui.isUpdated())
		{

			if ((pinFromGui) != trig_state)
			{
				trig_state = pinFromGui;
				if (pinFromGui)
				{
					midiMessage[0] = 0x90 | pinChan.getValue() - 1;
					midiMessage[1] = pinNote.getValue();
					midiMessage[2] = 100;
					pinMIDIOut.send(midiMessage, 3, 1);

				}

				if (!pinFromGui)
				{
					midiMessage[0] = 0x80 | pinChan.getValue() - 1;
					midiMessage[1] = pinNote.getValue();
					midiMessage[2] = 100;
					pinMIDIOut.send(midiMessage, 3, 1);
				}
			}
		}

	}

};

namespace
{
	auto r = Register<XCircle>::withId(L"xCircle");
}