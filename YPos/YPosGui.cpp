#include "mp_sdk_gui2.h"
#include "Drawing.h"

using namespace gmpi;
using namespace GmpiDrawing;

class YPosGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetMouseDown()
	{
		// pinMouseDown changed
	} 	
	void onPointerUp()
	{
		pinOnPointerDown = pinOnPointerMove;
	}	
	void onPointerMove()
	{
		invalidateRect();
	}
	void onMin()
	{
		invalidateRect();
	}
	void onMax()
	{
		invalidateRect();
	}

 	FloatGuiPin pinOnPointerDown;
 	FloatGuiPin pinOnPointerMove;
	FloatGuiPin pinOnPointerUp;
	FloatGuiPin pinValueShiftkey;
	FloatGuiPin pinMin;
	FloatGuiPin pinMax;
	BoolGuiPin pinMouseDown;
	BoolGuiPin pinShift;


public:
	YPosGui()
	{
		initializePin( pinOnPointerDown, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onSetMouseDown) );
		initializePin( pinOnPointerMove, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onPointerMove) );
		initializePin(pinOnPointerUp, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onPointerUp));
		initializePin(pinValueShiftkey, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onPointerMove));
		initializePin(pinMin, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onMin));
		initializePin(pinMax, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onMax));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onSetMouseDown));
		initializePin(pinShift, static_cast<MpGuiBaseMemberPtr2>(&YPosGui::onSetMouseDown));
	}
		
	int32_t onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{		
		float pointerDownValue = point.y;

		// Let host handle right-clicks.
		if ((flags & 0x10) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}
		else
			pinOnPointerDown = pointerDownValue;

		pinMouseDown = true;

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinShift = true;
		}
			setCapture();		

		invalidateRect();

		return gmpi::MP_OK;
	}

	//mouse up
	int32_t onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point) override

	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		pinOnPointerUp = pinOnPointerMove;

		pinMouseDown = false;

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinShift = false;
		}
		
		releaseCapture();

		invalidateRect();

		return gmpi::MP_OK;
	}
		
	//animation position
	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
			if (!getCapture())
			{
				return gmpi::MP_UNHANDLED;
			}

			PointL offset(point.x, pinOnPointerDown - point.y); // TODO overload subtraction.

			pinOnPointerMove = pinOnPointerUp + offset.y;

			if (pinOnPointerMove < pinMin)
				pinOnPointerMove = pinMin;

			if (pinOnPointerMove > pinMax)
				pinOnPointerMove = pinMax;

			if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
			{
				pinValueShiftkey = pinOnPointerMove;
			}

			invalidateRect();

			return gmpi::MP_OK;		
	}
};

namespace
{
	auto r = Register<YPosGui>::withId(L"YPos");
}
