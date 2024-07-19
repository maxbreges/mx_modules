#include "mp_sdk_gui2.h"
#include "Drawing.h"
#define _USE_MATH_DEFINES
#include <math.h>
//#include "../shared/fast_gamma.h"
#include "../shared/unicode_conversion.h"

using namespace gmpi;
using namespace gmpi_gui;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

class RectangleXGui final : public gmpi_gui::MpGuiGfxBase
{
public:
	FloatGuiPin pinCornerRadius;
 	BoolGuiPin pinTopLeft;
 	BoolGuiPin pinTopRight;
 	BoolGuiPin pinBottomLeft;
 	BoolGuiPin pinBottomRight;
 	StringGuiPin pinTopColor;
 	StringGuiPin pinBottomColor;
	BoolGuiPin pinMouseDown;
	StringGuiPin pinText;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	FloatGuiPin pinFontSize;

	void onSetMouseDown()
	{
	}

	void onSetText()
	{
		invalidateRect();
	}

	void onSetTextColor()
	{
		invalidateRect();
	}

	void onSetStyle()
	{
		invalidateRect();
	}


	RectangleXGui()
	{
		initializePin( pinCornerRadius, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinTopLeft, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinTopRight, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinBottomLeft, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinBottomRight, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinTopColor, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin( pinBottomColor, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw) );
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetMouseDown));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetText));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onSetStyle));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&RectangleXGui::onRedraw));

	}

	void RectangleXGui::onRedraw()
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

	int32_t RectangleXGui::OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext)
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
	

		//drawing text

		float font_size = -1;

		if (pinFontSize < 0.01f)

			{
				font_size = 0.01f;
			}
		
		else
		{
			font_size = (pinFontSize * 0.1f) * (height * 0.05f * width * 0.05f);
		}

		//delivering font
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat textFormat = g.GetFactory().CreateTextFormat(font_size, fontFace);

		textFormat.SetParagraphAlignment(ParagraphAlignment::Center),

			textFormat.SetTextAlignment(TextAlignment::Center);

		auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));


		auto displayText = dtext();

		
			g.DrawTextU(displayText, textFormat, getRect(), brush, 0);

		return gmpi::MP_OK;
	}

	std::string RectangleXGui::dtext()
	{
		return WStringToUtf8(pinText.getValue());
	}


	int32_t RectangleXGui::onPointerDown(int32_t flags, GmpiDrawing_API::MP1_POINT point)
	{
		// Let host handle right-clicks.
		if ((flags & GG_POINTER_FLAG_FIRSTBUTTON) == 0)
		{
			return gmpi::MP_OK; // Indicate successful hit, so right-click menu can show.
		}

		setCapture();

		pinMouseDown = true;

		return gmpi::MP_OK;

	}

	int32_t RectangleXGui::onPointerUp(int32_t flags, GmpiDrawing_API::MP1_POINT point)

	{
		if (!getCapture())
		{
			return gmpi::MP_UNHANDLED;
		}

		releaseCapture();

		pinMouseDown = false;

		return gmpi::MP_OK;
	}


};

namespace
{
	auto r = Register<RectangleXGui>::withId(L"My RectangleX");
}
