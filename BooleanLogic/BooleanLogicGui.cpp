#include "mp_sdk_gui2.h"

using namespace std;
using namespace gmpi;

class BooleanNOTGui final : public SeGuiInvisibleBase
{
    void onSetIn()
    {
        pinOut = !pinIn;
    }

	void onSetOut()
	{
		pinIn = !pinOut;
	}

	BoolGuiPin pinIn;
	BoolGuiPin pinOut;

public:
	BooleanNOTGui()
	{
		initializePin(pinIn, static_cast<MpGuiBaseMemberPtr2>(&BooleanNOTGui::onSetIn));
		initializePin(pinOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanNOTGui::onSetOut));
	}
};

class BooleanANDxGui final : public SeGuiInvisibleBase
{
    // IntGuiPin pinIntIn;             // This pin allows user input (can be ignored for auto allocation)
    BoolGuiPin pinBoolOut;          // Output pin
    vector<BoolGuiPin> pinSpareIn;  // Vector to hold input pins

public:
    BooleanANDxGui()
    {
        // Initialize output pin
        initializePin(pinBoolOut, static_cast<MpGuiBaseMemberPtr2>(&BooleanANDxGui::onSetSpareIn));
    }

    void onSetSpareIn()
    {
        // Perform AND operation when any input pin changes
        bool result = true; // Start with true for AND operation

        // Check the value of all input pins
        for (size_t i = 0; i < pinSpareIn.size(); i++)
        {
            bool inputValue = pinSpareIn[i]; // Get the value from the current input pin
            result = result && inputValue; // AND all inputs together
        }

        if (pinSpareIn.size())
        {
            pinBoolOut = result;
        }
    }

    int32_t initialize() override
    {
        // Retrieve total number of pins from the host
        int32_t pinCount = 0;
        // Access the base host interface (IMpUserInterfaceHost) to obtain pin count
        gmpi::IMpUserInterfaceHost* host = nullptr;
        if (getHost()->queryInterface(gmpi::MP_IID_UI_HOST, (void**)&host) == gmpi::MP_OK)
        {
            host->getPinCount(pinCount); // Safely call getPinCount
            host->release(); // Release the host interface when done
        }

        // Assuming that we reserve at least 2 pins (one output and one internal)
        const int numInputPins = max(0, pinCount - 1);

        try
        {
            pinSpareIn.resize(numInputPins); // Resize vector to hold input pins

            // Initialize each input pin in the vector
            for (size_t i = 0; i < numInputPins; ++i)
            {
                initializePin(pinSpareIn[i], static_cast<MpGuiBaseMemberPtr2>(&BooleanANDxGui::onSetSpareIn));
            }
        }
        catch (const std::exception&)
        {
            // Log or handle the exception if pin initialization fails
            return MP_FAIL; // Indicate failure
        }

        return SeGuiInvisibleBase::initialize(); // Ensure to call base class's initialize method
    }

    int32_t setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        // Call the base class's setPin method
        int32_t r = SeGuiInvisibleBase::setPin(pinId, voice, size, data);

        // If an input pin is changed, update and perform the AND operation
        if (pinId >= 0 && pinId < pinSpareIn.size())
        {
            onSetSpareIn();
        }

        return r;
    }
};

class ORGateGuiGui final : public SeGuiInvisibleBase
{
    void onSetBoolOut()
    {
        bool orResult = false;
        for (size_t i = 1; i < inValues.size(); ++i) { // Skip index 0 if it's not used for input
            orResult |= inValues[i];
        }
        // getHost()->pinTransmit(0, sizeof(orResult), &orResult);
        pinBoolOut = orResult;
    }

    BoolGuiPin pinBoolOut;

    MpGuiPin<bool> pinSpareIn;
    std::vector<bool> inValues;

public:

    // Declare static member
  //  static std::ofstream logFile;

    ORGateGuiGui()
    {
        initializePin(pinBoolOut);

        /*        // Initialize the log file
                if (!logFile.is_open())
                {
                    logFile.open("pinId_log.txt", std::ios::app);
                    if (!logFile)
                    {
                        // handle error if needed
                    }
                }*/
    }
    virtual int32_t MP_STDCALL setPin(int32_t pinId, int32_t voice, int32_t size, const void* data) override
    {
        /*        // Log info
                std::stringstream ss;
                ss << "pinId: " << pinId << ", size: " << size << std::endl;
                if (logFile.is_open()) {
                    logFile << ss.str() << "\n";
                    logFile.flush();
                }*/

        if (pinId == 0) {
            // This is the output pin, do not store in inValues
            bool value;
            VariableFromRaw<bool>(size, data, value);
            // Directly transmit the output value
            getHost()->pinTransmit(pinId, size, data);
            // Optionally, update internal state if needed
            // e.g., store last output value if necessary
        }
        else {
            // This is an input pin, store its value
            if (pinId >= (int)inValues.size()) {
                inValues.resize(pinId + 1, false);
            }
            bool value;
            VariableFromRaw<bool>(size, data, value);
            inValues[pinId] = value;

            // Recalculate OR result
            onSetBoolOut();
        }
        return 0;
    }
};

// Define static member
//std::ofstream ORGateGuiGui::logFile;

namespace
{
	bool r[] =
	{
		Register<BooleanNOTGui>::withId(L"NOTGate"),
	Register<BooleanANDxGui>::withId(L"AND Gate GuiROutLIn"),
	Register<BooleanANDxGui>::withId(L"AND Gate GuiLOutRIn"),
	Register<BooleanANDxGui>::withId(L"AND Gate GuiROutRIn"),
	Register<BooleanANDxGui>::withId(L"AND Gate GuiLOutLIn"),
    Register<ORGateGuiGui>::withId(L"OR Gate GuiROutLIn"),
    Register<ORGateGuiGui>::withId(L"OR Gate GuiLOutRIn"),
    Register<ORGateGuiGui>::withId(L"OR Gate GuiROutRIn"),
    Register<ORGateGuiGui>::withId(L"OR Gate GuiLOutLIn"),
	};
}
