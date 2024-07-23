#include "mp_sdk_gui2.h"
#include "Drawing.h"

using namespace gmpi;
using namespace GmpiDrawing;

class XCircleGui final : public gmpi_gui::MpGuiGfxBase
{	

	void onSetMouseOver()
	{
		invalidateRect();
	}

	void onSetMouseDown()
	{
		invalidateRect();
	}

 	void onSetColor()
	{
		invalidateRect();
	}

	BoolGuiPin pinMouseOver;
	BoolGuiPin pinMouseDown;	
 	StringGuiPin pinColor;

public:
	XCircleGui()
	{
		initializePin(pinMouseOver, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetMouseOver));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetMouseDown));
		initializePin( pinColor, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetColor) );
	}

	virtual int32_t MP_STDCALL setHover(bool isMouseOverMe) override
	{
		pinMouseOver = isMouseOverMe;

		invalidateRect();

		return gmpi::MP_OK;
	}

	//Mouse Down
	int32_t onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		// Let host handle right-clicks.
		if ((flags & 0x10) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}
		else

		{
			setCapture();

			if (pinMouseOver && 0x10)
				
			pinMouseDown = true;
		}

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

		releaseCapture();

		pinMouseDown = false;

		invalidateRect();

		return gmpi::MP_OK;
	}

//circles
	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		auto r = getRect();

		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * 0.4f;
		thickness = radius * 0.2f;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);

		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		auto brushBackground = g.CreateSolidColorBrush(Color::FromHexString(pinColor));

		Size circleSize1(radius, radius);

		{
			g.FillCircle(center, radius + thickness * 0.5f, brushBackground);
		}

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<XCircleGui>::withId(L"xCircle");
}
