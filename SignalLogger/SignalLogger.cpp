#include <vector>
#include <memory>
#include "mp_sdk_audio.h"
#include "PinIterator.h"

SE_DECLARE_INIT_STATIC_FILE(PhaseScope)
SE_DECLARE_INIT_STATIC_FILE(SignalLogger)

using namespace gmpi;

class SignalLogger final : public MpBase2
{
	BlobOutPin pinBlobOut;

	std::vector< std::unique_ptr<AudioInPin> > pinSignal;
	std::vector<float*> ins;
	std::vector<float> signalBuffer;
	int blockIndex = 0;

public:
	int recordingBufferSize_ = {};
	static const int recordingBufferHeaderFloats_ = 2;

	SignalLogger()
	{
		initializePin( pinBlobOut );
	}

	int32_t open() override
	{
		const int assumedGuiUpdateRateHz = 20;
		recordingBufferSize_ = static_cast<int>(host.getSampleRate()) / assumedGuiUpdateRateHz;

		// Register pins.
		PinIterator it(this);

		it.first();
		int pinIndex = 1;
		for (it.first(); !it.isDone(); ++it)
		{
			if ((*it)->getDatatype() == MP_BLOB)
				continue;

			pinSignal.push_back(std::make_unique<AudioInPin>());
//			initializePin((*it)->getUniqueId(), *(pinSignal.back()));
			initializePin(pinIndex++, *(pinSignal.back()));
		}

		signalBuffer.reserve(recordingBufferSize_ * it.size() + 2);
		ins.assign(pinSignal.size(), nullptr);

		initSignalBuffer();

		return MpBase2::open();
	}

	void subProcess( int sampleFrames )
	{
		for (int i = 0; i < pinSignal.size(); ++i)
		{
			ins[i] = getBuffer(*pinSignal[i]);
		}

		for (int s = 0; s < sampleFrames; ++s)
		{
			for (auto& in : ins)
			{
				signalBuffer.push_back(*in++);
			}

			if (signalBuffer.size() == recordingBufferSize_ * pinSignal.size() + recordingBufferHeaderFloats_)
			{
				// send to GUI.
				pinBlobOut.setValueRaw(signalBuffer.size() * sizeof(signalBuffer[0]), signalBuffer.data());
				pinBlobOut.sendPinUpdate(getBlockPosition() + s);

				initSignalBuffer();
			}
		}
	}

	void initSignalBuffer()
	{
		signalBuffer.clear();
		signalBuffer.push_back(static_cast<float>(blockIndex++));		// samples per channel
		signalBuffer.push_back(static_cast<float>(pinSignal.size()));	// number of channels
	}

	void onSetPins(void) override
	{
		setSubProcess(&SignalLogger::subProcess);
		setSleep(false);
	}
};

namespace
{
auto r = Register<SignalLogger>::withId(L"SE Signal Logger");
auto r2 = Register<SignalLogger>::withId(L"SE Phase Scope");
}
