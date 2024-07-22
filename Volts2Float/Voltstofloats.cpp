#include "mp_sdk_audio.h"

using namespace gmpi;

class VoltsToFloats : public MpBase2
{
    AudioInPin  pinIn;
    FloatOutPin pinOutF;

public:

    VoltsToFloats()
    {
        initializePin(pinIn);
        initializePin(pinOutF);
    }

    void subProcess(int sampleFrames)
    {
        float* in = getBuffer(pinIn);
       
        for (int s = 0; s < sampleFrames; s++) // Counts upward
        {
            pinOutF.setValue(*in * 10.f, getBlockPosition() + s); // Use getBlockPosition() function

            ++in; 
        }
    }

    void onSetPins(void) override
    {
        bool isStreaming = pinIn.isStreaming(); // Easier to write, cleaner to read

        setSubProcess(&VoltsToFloats::subProcess);

        setSleep(!isStreaming); // '!' means NOT. So, module sleeps when input is not streaming
    }
};

namespace
{
    auto r = Register<VoltsToFloats>::withId(L"My VoltsToFloats");
}
