#include "mp_sdk_gui2.h"
#include "Drawing.h"

using namespace gmpi;
using namespace GmpiDrawing;

class XCircleGui final : public gmpi_gui::MpGuiGfxBase
{
 	void onSetColor()
	{
		invalidateRect();
	}

 	StringGuiPin pinColor;

public:
	XCircleGui()
	{
		initializePin( pinColor, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetColor) );
	}

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
