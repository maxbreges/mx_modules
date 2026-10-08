
/* Copyright (c) 2007-2021 SynthEdit Ltd
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name SEM, nor SynthEdit, nor 'Music Plugin Interface' nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY SynthEdit Ltd ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL SynthEdit Ltd BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <list>
#include <vector>
#include "mp_sdk_gui2.h"
#include "Drawing.h"
#include "GraphHelpers.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace gmpi;
using namespace GmpiDrawing;

struct captureChunk
{
	captureChunk(int nothing) : signal(0), signal_peak(0) {}
	float* signal;
	float* signal_peak;
};

class PhaseScopeGui final : public gmpi_gui::MpGuiGfxBase
{
	const int maxRetainedChunks = 100;

	int channelCount = 2;
	std::vector<float> chunk;

	std::vector<Point> temp1; // to save reallocating every frame, we keep these around.
	std::vector<Point> temp2;

	void onSetBlob()
	{
		if (pinValueIn.rawSize() > sizeof(float) * 2)
		{
			const float* data = (const float*)pinValueIn.rawData();

			const auto chunkIndex = static_cast<int>(data[0]);
			channelCount = static_cast<int>(data[1]);

			if (chunkIndex == 0) // restarted audio?
				chunk.clear();

			const float* chunkData = data + 2;
			const size_t chunkSize = (pinValueIn.rawSize() / sizeof(float)) - 2;
			chunk.assign(chunkData, chunkData + chunkSize);
		}

		invalidateRect();
	}

	BlobGuiPin	pinValueIn;

public:
	PhaseScopeGui()
	{
		initializePin(pinValueIn, static_cast<MpGuiBaseMemberPtr2>(&PhaseScopeGui::onSetBlob));
	}

	int32_t MP_STDCALL OnRender(GmpiDrawing_API::IMpDeviceContext* drawingContext) override
	{
		Graphics g(drawingContext);
		ClipDrawingToBounds x(g, getRect());

		g.Clear(Color::Black);

		auto textFormat = GetGraphicsFactory().CreateTextFormat();
		auto brush = g.CreateSolidColorBrush(Color::Lime);

		auto r = getRect();

		if (chunk.empty())
			return gmpi::MP_OK;

		float height = r.bottom - r.top;
		float width = r.right - r.left;
		const float scale = (std::min)(width, height) * 0.46f;

		float mid_x = floorf(0.5f + width * 0.5f);
		float mid_y = floorf(0.5f + height * 0.5f);

		// calc transform to screen
		auto transform = Matrix3x2::Scale(-scale, -scale, { 0,0 });
		transform = transform * Matrix3x2::Translation(mid_x, mid_y);

		// draw grid
		{
			auto screen_mid_right = transform.TransformPoint({ 0, 0 });
			screen_mid_right.x = width;
			Point screen_mid_left{ 0, screen_mid_right.y };
			brush.SetColor(Color::Gray);
			g.DrawLine(screen_mid_left, screen_mid_right, brush);
		}

		const int traceCount = channelCount;
		const int framesPerChunk = static_cast<int>(chunk.size()) / traceCount;

		Color traceColors[] =
		{
			Color::Lime,
			Color::Yellow,
			Color::Red,
			Color::LightCyan,
		};

		const float sinNeg45 = sinf(M_PI * 0.25f);
		const float cosNeg45 = cosf(M_PI * 0.25f);
		{
			temp1.clear();
			temp2.clear();

			for (int i = 0 ; i < framesPerChunk; ++i)
			{
				Point pt(
					chunk[chunk.size() - traceCount * (i + 1)],
					chunk[chunk.size() - traceCount * (i + 1) + 1]
				);

				// Rotate point 45 degrees (better to pre-do? rather than re-apply repeatedly)
				Point p(
					pt.x * cosNeg45 - pt.y * sinNeg45,
					pt.x * sinNeg45 + pt.y * cosNeg45
					);

				if (isnan(p.y) || isinf(p.y))
				{
					p.y = 100.0; // Nans should stick out.
				}

				const auto p_screen = transform.TransformPoint(p);

				temp1.push_back(p_screen);
			}

			const float penWidth = 1;
			auto traceGeometry = DataToGraph(g, temp1);

			brush.SetColor(Color(0.f,1.f,0.f,1.0f));
			g.DrawGeometry(traceGeometry, brush, penWidth);
		}

		return gmpi::MP_OK;
	}
};

namespace
{
	auto r = Register<PhaseScopeGui>::withId(L"SE Phase Scope");
}
