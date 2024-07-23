#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "../shared/unicode_conversion.h"

using namespace gmpi;
using namespace GmpiDrawing;
using namespace JmUnicodeConversions;

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

	void onSetORIn()
	{
		invalidateRect();
	}

	void onToDSP()
	{
		invalidateRect();
	}

	void onTrigGui()
	{
		invalidateRect();
	}

 	void onSetColor()
	{
		invalidateRect();
	}

	void onSetTopCircleSize()
	{
		if (pinTopCircleSize > 1.f)
			pinTopCircleSize = 1.f;
		if (pinTopCircleSize < 0.f)
			pinTopCircleSize = 0.f;
		invalidateRect();
	}

	void onSetText()
	{
		invalidateRect();
	}

	void onSetTextb()
	{
		invalidateRect();
	}
	void onSetAcc()
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

	void onSetFontSize()
	{
		invalidateRect();
	}

	BoolGuiPin pinMouseOver;
	BoolGuiPin pinMouseDown;
	BoolGuiPin pinORIn;
	BoolGuiPin pinToDSP;
	BoolGuiPin pinTrigGui;
	StringGuiPin pinColor0;
 	StringGuiPin pinColor;
	FloatGuiPin pinTopCircleSize;
	StringGuiPin pinText;
	StringGuiPin pinTextb;
	BoolGuiPin pinSharpFlat;
	StringGuiPin pinTextColor;
	StringGuiPin pinFont;
	FloatGuiPin pinFontSize;

public:
	XCircleGui()
	{
		initializePin(pinMouseOver, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetMouseOver));
		initializePin(pinMouseDown, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetMouseDown));
		initializePin(pinORIn, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetORIn));
		initializePin(pinToDSP, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onToDSP));
		initializePin(pinTrigGui, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onTrigGui));
		initializePin(pinColor0, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetColor));
		initializePin(pinColor, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetColor) );
		initializePin(pinTopCircleSize, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetTopCircleSize));
		initializePin(pinText, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetText));
		initializePin(pinTextb, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetTextb));
		initializePin(pinSharpFlat, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetAcc));
		initializePin(pinTextColor, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetTextColor));
		initializePin(pinFont, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetStyle));
		initializePin(pinFontSize, static_cast<MpGuiBaseMemberPtr2>(&XCircleGui::onSetFontSize));


	}

	virtual int32_t MP_STDCALL setHover(bool isMouseOverMe) override
	{
		pinMouseOver = isMouseOverMe;

		if (!pinMouseOver)
			pinToDSP = false;
		if (pinORIn && pinMouseOver)
			pinToDSP = true;

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
				pinToDSP = true;
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
		pinToDSP = false;

		invalidateRect();

		return gmpi::MP_OK;
	}

	//circles
	//bg circle dimensions

	void calcDimensions(Point& center, float& radius, float& thickness)
	{
		auto r = getRect();

		center = Point((r.left + r.right) * 0.5f, (r.top + r.bottom) * 0.5f);
		radius = (std::min)(r.getWidth(), r.getHeight()) * 0.4f;
		thickness = radius * 0.2f;
	}

	//top circle dimensions

	void calcDimensions1(Point& center1, float& radius1, float& thickness1)
	{
		float mult;
		auto r1 = getRect();
		mult = (pinTopCircleSize) * 0.4f;
		center1 = Point((r1.left + r1.right) * 0.5f, (r1.top + r1.bottom) * 0.5f);
		radius1 = (std::min)(r1.getWidth(), r1.getHeight()) * mult;
		thickness1 = radius1 * 0.2f;
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext ) override
	{
		Graphics g(drawingContext);

		//bg circle
		Point center;
		float radius;
		float thickness;
		calcDimensions(center, radius, thickness);

		if ((pinORIn && pinMouseOver) || (pinMouseDown && pinMouseOver) || (pinTrigGui))
		{
			auto brushBackground = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

			if ((!pinORIn) || (!pinMouseOver) || (!pinMouseDown) || (!pinTrigGui))
			{
				auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinColor0));
			}

			Size circleSize1(radius, radius);

			{
				g.FillCircle(center, radius + thickness * 0.5f, brushBackground);
			}

		}

		//top circle

		Point center1;
		float radius1;
		float thickness1;
		calcDimensions1(center1, radius1, thickness1);

		if ((pinORIn && pinMouseOver) || (pinMouseDown && pinMouseOver) || (pinTrigGui))
		{
			auto brushTopColor = g.CreateSolidColorBrush(Color::FromHexString(pinColor));

			if ((!pinORIn) || (!pinMouseOver) || (!pinMouseDown) || (!pinTrigGui))
			{
				auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinColor0));
			}

			Size circleSize1(radius1, radius1);

			{
				g.FillCircle(center1, radius1 + thickness1 * 0.5f, brushTopColor);
			}
		}


		//drawing text
		float font_size = -1;

		if (pinFontSize >= 0.01f)
		{
			if (pinFontSize <= 10.f)
			{
				font_size = (pinFontSize * 0.16f) * radius;
			}
			if (pinFontSize > 10.f)
			{
				font_size = 10.f;
			}
			if (pinFontSize < 0.01f)
			{
				font_size = 0.01f;
			}
		}
		else
		{
			font_size = radius;
		}

		//delivering font
		std::string str = { pinFont };
		const char* fontFace = str.c_str();
		TextFormat textFormat = g.GetFactory().CreateTextFormat(font_size, fontFace);

		textFormat.SetParagraphAlignment(ParagraphAlignment::Center),

			textFormat.SetTextAlignment(TextAlignment::Center);

		if ((pinORIn && pinMouseOver) || (pinMouseDown && pinMouseOver) || (pinTrigGui))
		{
			auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinTextColor));

			if ((!pinORIn) || (!pinMouseOver) || (!pinMouseDown) || (!pinTrigGui))
			{
				auto brush = g.CreateSolidColorBrush(Color::FromHexString(pinColor0));
			}

			auto displayText = acc();

			g.DrawTextU(displayText, textFormat, getRect(), brush, 0);

		}

		return gmpi::MP_OK;
	}

	std::string acc()
	{
		if (pinSharpFlat)
		{
			return WStringToUtf8(pinTextb.getValue());
		}
		else
			return WStringToUtf8(pinText.getValue());
	}

};

namespace
{
	auto r = Register<XCircleGui>::withId(L"xCircle");
}
