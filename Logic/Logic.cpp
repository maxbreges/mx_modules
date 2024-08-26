/* Copyright (c) 2007-2023 SynthEdit Ltd

Permission to use, copy, modify, and /or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#include <memory>
#include "mp_sdk_audio.h"
#include "PinIterator.h"

using namespace gmpi;

class NOT : public MpBase2
{
protected:
	BoolInPin pinInput;
	BoolOutPin pinOutput;

public:
	NOT()
	{
		initializePin(pinInput);
		initializePin(pinOutput);
	}
	void onSetPins() override
	{
		pinOutput = !pinInput.getValue();
	}
};
namespace
{
	auto r0 = Register<NOT>::withId(L"SE NOT");
}

// multi ins, one out.
class Logic : public MpBase2
{
protected:
	BoolOutPin pinOutput;
	std::vector< std::unique_ptr<BoolInPin> > pinInputs;

public:
	Logic()
	{
		initializePin(pinOutput);
	}

	int32_t open() override
	{
		// Register pins.
		PinIterator it(this);

		it.first();
		int pinIndex = 1;
		for (it.first(); !it.isDone(); ++it)
		{
			if ((*it)->getDirection() == MP_OUT)
				continue;

			pinInputs.push_back(std::make_unique<BoolInPin>());
			initializePin(pinIndex++, *(pinInputs.back()));
		}

		return MpBase2::open();
	}
};

class AND final : public Logic
{
public:
	void onSetPins() override
	{
		bool res{ true };
		for (auto& p : pinInputs)
		{
			if (!p->getValue())
			{
				res = false;
				break;
			}
		}
		pinOutput = res;
	}
};

namespace
{
	auto r1 = Register<AND>::withId(L"SE AND");
}

class OR final : public Logic
{
public:
	void onSetPins() override
	{
		bool res{ false };
		for (auto& p : pinInputs)
		{
			if (p->getValue())
			{
				res = true;
				break;
			}
		}
		pinOutput = res;
	}
};

namespace
{
	auto r2 = Register<OR>::withId(L"SE OR");
}

class NAND final : public Logic
{
public:
	void onSetPins() override
	{
		bool res{ false };
		for (auto& p : pinInputs)
		{
			if (!p->getValue())
			{
				res = true;
				break;
			}
		}
		pinOutput = res;
	}
};

namespace
{
	auto r3 = Register<NAND>::withId(L"SE NAND");
}

class NOR final : public Logic
{
public:
	void onSetPins() override
	{
		bool res{ true };
		for (auto& p : pinInputs)
		{
			if (p->getValue())
			{
				res = false;
				break;
			}
		}
		pinOutput = res;
	}
};

namespace
{
	auto r4 = Register<NOR>::withId(L"SE NOR");
}

class XOR final : public Logic
{
public:
	void onSetPins() override
	{
		// In general, an XOR gate gives an output value of 1
		// when there are an odd number of 1's on the inputs to the gate
		bool res{ false };
		for (auto& p : pinInputs)
		{
			if (p->getValue())
			{
				res = !res;
			}
		}
		pinOutput = res;
	}
};

namespace
{
	auto r5 = Register<XOR>::withId(L"SE XOR");
}
