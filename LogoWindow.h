#pragma once

//=============================================================================
/* IPlug2 Includes */
#include "config.h"
#include "IControl.h"
//=============================================================================

using namespace iplug;
using namespace igraphics;

class LogoWindow : public IControl
{
public:
	LogoWindow(const IRECT& bounds) : IControl(bounds)
	{}

	void Draw(IGraphics& g) override
	{
		g.FillRect(IColor(255, 0, 0, 0), mRECT, &BLEND_50); // black background with 50% alpha blend
		g.FillRoundRect(IColor(255, 20, 20, 20), mRECT.GetPadded(-300.0f, -30.0f, -300.0f, -30.0f), 5.0f, &BLEND_95);
		const IBitmap b = g.LoadBitmap(LOGO_FN); // Load Logo bitmap from config.h and resources
		g.DrawBitmap(b, mRECT.GetPadded(-300, -30, -300, -30).GetCentredInside(250.0f).GetHShifted(25.0f).GetVShifted(-40.0f));

		// Draw plugin name from config.h
		char plugin_name[1024];
		sprintf(plugin_name, "%s", PLUG_NAME);
		g.DrawText(IText(24, EAlign::Center, IColor(255, 245, 245, 245)),
			plugin_name,
			mRECT.GetPadded(-300.0f, -30.0f, -300.0f, -30.0f)
			.GetCentredInside(250.0f)
			.GetVShifted(40.0f)
			.GetHShifted(0.0f));

		// Draw version number from config.h
		char version[1024];
		sprintf(version, "Version: %s", PLUG_VERSION_STR);
		g.DrawText(IText(24, EAlign::Center, IColor(255, 245, 245, 245)),
			version,
			mRECT.GetPadded(-300.0f, -30.0f, -300.0f, -30.0f)
			.GetCentredInside(250.0f)
			.GetVShifted(90.0f)
			.GetHShifted(0.0f));

		// Draw copyright from config.h
		char copyright[1024];
		sprintf(copyright, "%s", PLUG_COPYRIGHT_STR);
		g.DrawText(IText(16, EAlign::Center, IColor(255, 245, 245, 245)),
			copyright,
			mRECT.GetPadded(-300.0f, -30.0f, -300.0f, -30.0f)
			.GetCentredInside(250.0f)
			.GetVShifted(120.0f)
			.GetHShifted(0.0f));
	}

	// Delete control when clicked upon it.
	void OnMouseDown(float x, float y, const IMouseMod& mod) override
	{
		GetUI()->RemoveControl(this);
	}

private:
	const IBlend BLEND_95 = IBlend(EBlend::Default, 0.95f);
	const IBlend BLEND_90 = IBlend(EBlend::Default, 0.90f);
	const IBlend BLEND_85 = IBlend(EBlend::Default, 0.85f);
	const IBlend BLEND_80 = IBlend(EBlend::Default, 0.80f);
};