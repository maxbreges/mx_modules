#include "mp_sdk_audio.h"
#include "mp_midi.h"

SE_DECLARE_INIT_STATIC_FILE(SeparateChannels);

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class SeparateChannels final : public MpBase2
{
	MidiInPin pinMIDIIn;
	BoolOutPin pinChan1;
	BoolOutPin pinChan2;
	BoolOutPin pinChan3;
	BoolOutPin pinChan4;

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;
	int chan = 0;

public:
	SeparateChannels() :
		// init the midi converter
		midiConverter(
			// provide a lambda to accept converted MIDI 2.0 messages
			[this](const midi::message_view& msg, int offset)
			{
				onMidi2Message(msg);
			}
		)

	{
		initializePin(pinMIDIIn);
		initializePin(pinChan1);
		initializePin(pinChan2);
		initializePin(pinChan3);
		initializePin(pinChan4);
	}

	int32_t open() override
	{
		MpBase2::open();    // always call the base class
		return gmpi::MP_OK;
	}

	// put your midi handling code in here.
	void onMidi2Message(const midi::message_view& msg)
	{
		const auto header = gmpi::midi_2_0::decodeHeader(msg);

		messageSize = (int)(size_t)msg.size();
		// only 8-byte messages supported. only 16 channels supported
		if (header.messageType != gmpi::midi_2_0::ChannelVoice64)
			return;

		chan = static_cast<uint8_t>(msg[1] & 0x0f);

	}

	// passes all MIDI to the converter.
	void onMidiMessage(int pin, unsigned char* midiMessage, int size) override
	{
		midi::message_view msg((const uint8_t*)midiMessage, size);
		// convert everything to MIDI 2.0
		midiConverter.processMidi(msg, -1);

		//chan = static_cast<uint8_t>(msg[1] & 0x0f);

		if (chan == 0)
		{
			pinChan1 = true;
		}
		else
			pinChan1 = false;

		if (chan == 1)
		{
			pinChan2 = true;
		}
		else
			pinChan2 = false;

		if (chan == 2)
		{
			pinChan3 = true;
		}
		else
			pinChan3 = false;

		if (chan == 3)
		{
			pinChan4 = true;
		}
		else
			pinChan4 = false;
	}

};

namespace
{
	auto r = Register<SeparateChannels>::withId(L"mx MIDIChanGate");
}



