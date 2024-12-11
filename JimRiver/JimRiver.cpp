#include "mp_sdk_audio.h"
#include "mp_midi.h"
#include <array>

SE_DECLARE_INIT_STATIC_FILE(JimRiverNewbie);

#define NOTE_ON     0x90
#define NOTE_OFF    0x80

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;


class JimRiver final : public MpBase2
{
	MidiInPin pinMIDIIn;
	IntInPin pinTranspose;
	BoolOutPin pinKick;
	BoolOutPin pinSnare;
	BoolOutPin pinTom;
	BoolOutPin pinHiHat;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;
	int chan = 0;
	int Note = 0;
	
public:
	JimRiver() :
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
		initializePin(pinTranspose);
		initializePin( pinKick );
		initializePin( pinSnare );
		initializePin(pinTom);
		initializePin(pinHiHat);
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

		chan = static_cast<uint8_t>(msg[1] & 0x0f);

		const auto note = gmpi::midi_2_0::decodeNote(msg);
		Note = (int)(uint8_t)note.noteNumber;
		
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;

		switch (pinTranspose)
		{
		case 0: Note = Note + 24;  break;
		case 1: Note = Note + 12; break;
		case 2 : Note = Note; break;
		case 3: Note = Note - 12; break;
		case 4: Note = Note - 24; break;
		case 5: Note = Note - 36; break;
		case 6: Note = Note - 48; break;
		}
	
		{
			switch (header.status)
			{
			case gmpi::midi_2_0::NoteOn:
			{
				int i = Note;

				switch (i)
				{
				case 36: pinKick = true;  break;
				case 38: pinSnare = true; break;
				case 41: pinTom = true; break;
				case 42: pinHiHat = true; break;
				}				
			}
			break;

			case gmpi::midi_2_0::NoteOff:
			{
				pinKick = false;
				pinSnare = false;
				pinTom = false;
				pinHiHat = false;
			}
			break;
			}
		}
	}

	void onSetPins() override
	{

	}
};

namespace
{
	auto r = Register<JimRiver>::withId(L"JimRiver");
}
