#include "mp_sdk_audio.h"

using namespace gmpi;

class VoltsToFloats : public MpBase2
{
    AudioInPin  pinIn;
    AudioOutPin pinOutV; // Just to control Bypass
    FloatOutPin pinOutF;

public:

    VoltsToFloats()
    {
        initializePin(pinIn);
        initializePin(pinOutV);
        initializePin(pinOutF);
    }

    void subProcess(int sampleFrames)
    {
        float* in = getBuffer(pinIn);
        float* outV = getBuffer(pinOutV);


        for (int s = 0; s < sampleFrames; s++) // Counts upward
        {
            pinOutF.setValue(*in * 10.f, getBlockPosition() + s); // Use getBlockPosition() function

            *outV++ = *in++; // Shorter version of pointer increment, add ++ to last instance of each pointer
        }
    }

    void onSetPins(void) override
    {
        bool isStreaming = pinIn.isStreaming(); // Easier to write, cleaner to read

        setSubProcess(&VoltsToFloats::subProcess);

        pinOutV.setStreaming(isStreaming); // Output is streaming only when input is streaming

        setSleep(!isStreaming); // '!' means NOT. So, module sleeps when input is not streaming
    }
};

namespace
{
    auto r = Register<VoltsToFloats>::withId(L"My VoltsToFloats");
}
