#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;

class RectangleXGui final : public gmpi_gui::MpGuiGfxBase
{
public:
	FloatGuiPin pinOnPointerDown;
	FloatGuiPin pinOnPointerMoveShift;
	FloatGuiPin pinOnPointerMoveCtrl;
	FloatGuiPin pinOnPointerUp;
	FloatGuiPin pinOnPointerUpCtrl;
	FloatGuiPin pinCornerRadius;
 	BoolGuiPin pinTopLeft;
 	BoolGuiPin pinTopRight;
 	BoolGuiPin pinBottomLeft;
 	BoolGuiPin pinBottomRight;
 	StringGuiPin pinTopColor;
 	StringGuiPin pinBottomColor;
	FloatGuiPin pinMin;
	FloatGuiPin pinMax;
	FloatGuiPin pinMinCtrl;
	FloatGuiPin pinMaxCtrl;
	BoolGuiPin pinMouseDown;
	BoolGuiPin pinShift;
	BoolGuiPin pinCtrl;

	void onSetMouseDown()
	{
		invalidateRect();
	}
	void onPointerUp()
	{
		//pinOnPointerDown = pinOnPointerMoveShift;
		//pinOnPointerDown = pinOnPointerMoveShift;
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

	void onMinCtrl()
	{
		invalidateRect();
	}
	void onMaxCtrl()
	{
		invalidateRect();
	}

	RectangleXGui()
	{
		initializePin(pinOnPointerDown, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetMouseDown));
		initializePin(pinOnPointerMoveShift, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onPointerMove));
		initializePin(pinOnPointerMoveCtrl, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onPointerMove));
		initializePin(pinOnPointerUp, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onPointerUp));
		initializePin(pinOnPointerUpCtrl, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onPointerUp));
		initializePin( pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinTopLeft, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinTopRight, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinBottomLeft, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinBottomRight, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinBottomColor, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin(pinMin, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onMin));
		initializePin(pinMax, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onMax));
		initializePin(pinMinCtrl, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onMinCtrl));
		initializePin(pinMaxCtrl, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onMaxCtrl));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetMouseDown));
		initializePin(pinShift, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetMouseDown));
		initializePin(pinCtrl, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetMouseDown));
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
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinCtrl = true;
		}
		setCapture();

		invalidateRect();

		return gmpi::MP_OK;
	}


	//onPointerMove
	int32_t MP_STDCALL onPointerMove(int32_t flags, GmpiDrawing_API::MP1_POINT point) override
	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{

			PointL offset(point.x, pinOnPointerDown - point.y); // TODO overload subtraction.

			pinOnPointerMoveShift = pinOnPointerUp + offset.y;

			if (pinOnPointerMoveShift < pinMin)
				pinOnPointerMoveShift = pinMin;

			if (pinOnPointerMoveShift > pinMax)
				pinOnPointerMoveShift = pinMax;
		}

		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			PointL offset1(point.x, pinOnPointerDown - point.y); // TODO overload subtraction.

			pinOnPointerMoveCtrl = pinOnPointerUpCtrl + offset1.y;

			if (pinOnPointerMoveCtrl < pinMinCtrl)
				pinOnPointerMoveCtrl = pinMinCtrl;

			if (pinOnPointerMoveCtrl > pinMaxCtrl)
				pinOnPointerMoveCtrl = pinMaxCtrl;
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

		pinOnPointerUp = pinOnPointerMoveShift;
		pinOnPointerUpCtrl = pinOnPointerMoveCtrl;

		pinMouseDown = false;
		pinCtrl = false;
		pinShift = false;

	/*
		if (flags & gmpi_gui_api::GG_POINTER_KEY_SHIFT)
		{
			pinShift = false;
		}
		if (flags & gmpi_gui_api::GG_POINTER_KEY_CONTROL)
		{
			pinCtrl = false;
		}*/

		releaseCapture();

		invalidateRect();

		return gmpi::MP_OK;
	}


	void onRedraw()
	{
		invalidateRect();
	}


	Color FromHexStringBackwardCompatible(const std::wstring& s)
	{
		constexpr float oneOver255 = 1.0f / 255.0f;

		wchar_t* stopString;
		uint32_t hex = wcstoul(s.c_str(), &stopString, 16);
		float alpha = (hex >> 24) * oneOver255;

		return Color(se_sdk::FastGamma::sRGB_to_float((hex >> 16) & 0xff), se_sdk::FastGamma::sRGB_to_float((hex >> 8) & 0xff), se_sdk::FastGamma::sRGB_to_float(hex & 0xff), alpha);
	}

	int32_t OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext)
	{
		Graphics g(drawingContext);

		auto r = getRect();
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		auto topCol = FromHexStringBackwardCompatible(pinTopColor);
		auto botCol = topCol;
		if (!pinBottomColor.getValue().empty())
		{
			botCol = FromHexStringBackwardCompatible(pinBottomColor);
		}

		int radius = (int)pinCornerRadius;

		radius = (std::min)(radius, width / 2);
		radius = (std::min)(radius, height / 2);

		auto geometry = g.GetFactory().CreatePathGeometry();
		auto sink = geometry.Open();

		// define a corner 
		const float rightAngle = M_PI * 0.5f;
		// top left
		if (pinTopLeft)
		{
			sink.BeginFigure(Point(0, radius), FigureBegin::Filled);
			ArcSegment as(Point(radius, 0), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.BeginFigure(Point(0, 0), FigureBegin::Filled);
		}
		/*
		// tweak needed for radius of 10
		if(radius == 20)
		{
		Corner.Width += 1;
		Corner.Height += 1;
		width -=1; height -= 1;
		}
		*/
		// top right
		if (pinTopRight)
		{
			sink.AddLine(Point(width - radius, 0));
			//		sink.AddArc(Corner, 270, 90);
			ArcSegment as(Point(width, radius), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.AddLine(Point(width, 0));
		}

		// bottom right
		if (pinBottomRight)
		{
			sink.AddLine(Point(width, height - radius));
			//		sink.AddArc(Corner, 0, 90);
			ArcSegment as(Point(width - radius, height), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.AddLine(Point(width, height));
		}

		// bottom left
		if (pinBottomLeft)
		{
			sink.AddLine(Point(radius, height));
			ArcSegment as(Point(0, height - radius), Size(radius, radius), rightAngle);
			sink.AddArc(as);
		}
		else
		{
			sink.AddLine(Point(0, height));
		}

		// end path
		sink.EndFigure();
		sink.Close();

		Point point1(1, 0);
		Point point2(1, height);

		GradientStop gradientStops[]
		{
			{ 0.0f, topCol },
			{ 1.0f, botCol },
		};

		auto gradientBrush = g.CreateLinearGradientBrush(gradientStops, point1, point2);

		g.FillGeometry(geometry, gradientBrush);

		return gmpi::MP_OK;
	}

};

namespace
{
	auto r = Register<RectangleXGui>::withId(L"My RectangleX");
}
