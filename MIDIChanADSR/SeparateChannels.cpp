#include "mp_sdk_audio.h"
#include "mp_midi.h"

SE_DECLARE_INIT_STATIC_FILE(SeparateChannels);

#define NOTE_ON     0x90
#define NOTE_OFF    0x80

using namespace gmpi;
using namespace GmpiMidi;
using namespace midi_2_0;

class SeparateChannels final : public MpBase2
{
	MidiInPin pinMIDIIn;
	AudioInPin pinADSR;
	AudioOutPin pinChan1;
	AudioOutPin pinChan2;
	AudioOutPin pinChan3;
	AudioOutPin pinChan4;	

	gmpi::midi_2_0::MidiConverter2 midiConverter;

	int messageSize = 0;
	int chan = 0;
	float adsr = 0.f;

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
		initializePin(pinADSR);
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
	}

	void subProcess(int sampleFrames)
	{
		// get pointers to in/output buffers.
		auto adsr_ptr = getBuffer(pinADSR);
		auto chan1_ptr = getBuffer(pinChan1);
		auto chan2_ptr = getBuffer(pinChan2);
		auto chan3_ptr = getBuffer(pinChan3);
		auto chan4_ptr = getBuffer(pinChan4);
		
		for (int s = sampleFrames; s > 0; --s)
		{
			adsr = *adsr_ptr;	// get the sample 'POINTED TO' by in1.
			
			// Multiplying the two input's samples together.
			if (chan == 0)
			{
				*chan1_ptr = adsr;
				*chan2_ptr = 0.f;
				*chan3_ptr = 0.f;
				*chan4_ptr = 0.f;
			}
			if (chan == 1)
			{
				*chan2_ptr = adsr;
				*chan1_ptr = 0.f;
				*chan3_ptr = 0.f;
				*chan4_ptr = 0.f;
			}
			if (chan == 2)
			{
				*chan3_ptr = adsr;
				*chan1_ptr = 0.f;
				*chan2_ptr = 0.f;
				*chan4_ptr = 0.f;
			}
			if (chan == 3)
			{
				*chan4_ptr = adsr;
				*chan1_ptr = 0.f;
				*chan2_ptr = 0.f;
				*chan3_ptr = 0.f;
			}
			// increment the pointers (move to next sample in buffers).
			++adsr_ptr;
			++chan1_ptr;
			++chan2_ptr;
			++chan3_ptr;
			++chan4_ptr;
		}
	}

	void onSetPins(void) override
	{	
		if (pinADSR.isStreaming())
		{			
		}
		setSubProcess(&SeparateChannels::subProcess);
	}
};

namespace
{
	auto r = Register<SeparateChannels>::withId(L"mx MIDIChanADSR");
}



