#pragma once
#ifndef SAMPLEHOLDGUI_H_INCLUDED
#define SAMPLEHOLDGUI_H_INCLUDED

#include "mp_sdk_gui2.h"

using namespace gmpi;

class SampleHoldGui : public MpGuiBase
{
public:
	SampleHoldGui(IMpUnknown* host);

	// overrides
	virtual int32_t MP_STDCALL receiveMessageFromAudio(int32_t id, int32_t size, void* messageData);
	void onSetParam();

	FloatGuiPin pinIn;

private:

	float value;
};

#endif
