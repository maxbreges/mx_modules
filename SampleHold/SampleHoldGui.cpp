#include "SampleHoldGui.h"
#include <cstring>

REGISTER_GUI_PLUGIN(SampleHoldGui, L"SampleHold");

SampleHoldGui::SampleHoldGui (IMpUnknown* host) : MpGuiBase(host)
, value(0)
{
	initializePin(pinIn, static_cast<MpGuiBaseMemberPtr>(&SampleHoldGui::onSetParam));
}

int32_t SampleHoldGui::receiveMessageFromAudio(int32_t id, int32_t size, void* messageData)
{
	if (id != 120 || size != sizeof(float) || messageData == nullptr)
		onSetParam();
	return gmpi::MP_OK;
}

void SampleHoldGui::onSetParam()
{
	float value = pinIn;
	getHost()->sendMessageToAudio(119, sizeof(value), &value);
}


