#include "mp_sdk_audio.h"
#include "mp_midi.h"

SE_DECLARE_INIT_STATIC_FILE(NoteOffToBool);

#define NOTE_OFF    0x80

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class NoteOffToBool final : public MpBase2
{
	MidiInPin pinMIDIIn;

private:
	BoolOutPin pinToGui;

public:

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;

	NoteOffToBool() :
		// init the midi converter
		midiConverter(
			// provide a lambda to accept converted MIDI 2.0 messages
			[this](const midi::message_view& msg, int offset)
			{
				onMidi2Message(msg);
			}
		)
	{
		initializePin( pinMIDIIn );		
		initializePin(pinToGui);
	}

	int32_t open() override
	{
		MpBase2::open();    // always call the base class
		return gmpi::MP_OK;
	}

	// passes all MIDI to the converter.
	void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
	{
		midi::message_view msg((const uint8_t*)midiMessage, size);

		// convert everything to MIDI 2.0
		midiConverter.processMidi(msg, -1);
	}

	// put your midi handling code in here.
	void onMidi2Message(const midi::message_view& msg)
	{
		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		messageSize = (int)(size_t)msg.size();
		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;


		switch (header.status)
		{
	
		case gmpi::midi_2_0::NoteOff:
		{
			const auto note = gmpi::midi_2_0::decodeNote(msg);
			//pinNote = (int)(uint8_t)note.noteNumber;
			pinToGui = (float)(note.velocity);
		}
		break;
		};
	}

};

namespace
{
	auto r = Register<NoteOffToBool>::withId(L"NoteOffToBool");
}
