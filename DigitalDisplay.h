// Copyright 2022 RAR-AUDIO. All Rights Reserved.
#pragma once

//=============================================================================
/* Standard Includes */
#include <array>
//=============================================================================
/* IPlug2 Includes */
#include "IControl.h"
//=============================================================================

using namespace iplug;
using namespace igraphics;

struct DigitalDisplayColors
{
    IColor background_color = IColor (255, 63, 0, 0);
    IColor color = IColor (255, 255, 0, 0);
    IColor blur_1_color = IColor (255, 95, 95, 191);
    IColor blur_2_color = IColor (255, 47, 47, 191);

    DigitalDisplayColors() = default;

    DigitalDisplayColors (const std::array<unsigned char, 4>& background_color_hex,
                          const std::array<unsigned char, 4>& color_hex,
                          const std::array<unsigned char, 4>& blur_1_color_hex,
                          const std::array<unsigned char, 4>& blur_2_color_hex)
    {
        background_color = IColor (background_color_hex[0], background_color_hex[1], background_color_hex[2], background_color_hex[3]);
        color = IColor (color_hex[0], color_hex[1], color_hex[2], color_hex[3]);
        blur_1_color = IColor (blur_1_color_hex[0], blur_1_color_hex[1], blur_1_color_hex[2], blur_1_color_hex[3]);
        blur_2_color = IColor (blur_2_color_hex[0], blur_2_color_hex[1], blur_2_color_hex[2], blur_2_color_hex[3]);
    }
};

class DigitalDisplay : public IControl
{
public:
    DigitalDisplay (const IRECT& bounds, const char* text, const IText& text_style = DEFAULT_TEXT)
        : IControl (bounds),
          text_ (text)
    {
        mText = text_style;
        back_digits_ = std::string (*text, '~');
    }

    void Draw (IGraphics& g) override
    {
        g.FillRect (colors_.background_color, mRECT, &BLEND_95);
        g.FillRect (colors_.blur_1_color, mRECT, &BLEND_01);
        g.FillRect (colors_.blur_2_color, mRECT, &BLEND_08);
        g.DrawText (mText, back_digits_.c_str(), mRECT, &BLEND_20);

        g.DrawText (mText, text_.c_str(), mRECT, nullptr);
    }

private:
    std::string text_;
    DigitalDisplayColors colors_;
    std::string back_digits_;

    const IBlend BLEND_95 = IBlend (EBlend::Default, 0.95f);
    const IBlend BLEND_90 = IBlend (EBlend::Default, 0.90f);
    const IBlend BLEND_85 = IBlend (EBlend::Default, 0.85f);
    const IBlend BLEND_80 = IBlend (EBlend::Default, 0.80f);
    const IBlend BLEND_20 = IBlend (EBlend::Default, 0.20f);
    const IBlend BLEND_15 = IBlend (EBlend::Default, 0.15f);
    const IBlend BLEND_08 = IBlend (EBlend::Default, 0.08f);
    const IBlend BLEND_01 = IBlend (EBlend::Default, 0.01f);
};

class DigitalDisplayCaption : public ITextControl
{
public:
    DigitalDisplayCaption (const IRECT& bounds, int paramIdx, const IText& text = DEFAULT_TEXT, bool draw_frame = true)
        : ITextControl (bounds, "", text, colors_.background_color),
          m_draw_frame_ (draw_frame)
    {
        IControl::SetParamIdx (paramIdx);
        mDblAsSingleClick = true;
        mDisablePrompt = false;
        mIgnoreMouse = false;
        mText = text;

        back_digits_ = std::string (256ull, '~');
    }

    void Draw (IGraphics& g) override
    {
        if (const auto p_param = GetParam())
        {
            p_param->GetDisplay (mStr);
        }

        if (m_draw_frame_)
            g.DrawRect (COLOR_BLACK, mRECT, nullptr, 1.0);

        g.FillRect (IColor (255, 17, 17, 17), mRECT, nullptr);
        g.FillRect (colors_.background_color, mRECT, &BLEND_95);
        g.FillRect (colors_.color, mRECT, &BLEND_20);
        g.FillRect (colors_.blur_1_color, mRECT, &BLEND_01);
        g.FillRect (colors_.blur_2_color, mRECT, &BLEND_08);
        g.DrawText (mText, back_digits_.c_str(), mRECT, &BLEND_20);

        g.DrawText (mText, mStr.Get(), mRECT, nullptr);
    }

    void OnMouseDown (float x, float y, const IMouseMod& mod) override
    {
        if (mod.L || mod.R)
            PromptUserInput (mRECT);
    }

private:
    std::string back_digits_;
    DigitalDisplayColors colors_;

    bool m_draw_frame_;

    const IBlend BLEND_95 = IBlend (EBlend::Default, 0.95f);
    const IBlend BLEND_90 = IBlend (EBlend::Default, 0.90f);
    const IBlend BLEND_85 = IBlend (EBlend::Default, 0.85f);
    const IBlend BLEND_80 = IBlend (EBlend::Default, 0.80f);
    const IBlend BLEND_20 = IBlend (EBlend::Default, 0.20f);
    const IBlend BLEND_15 = IBlend (EBlend::Default, 0.15f);
    const IBlend BLEND_08 = IBlend (EBlend::Default, 0.08f);
    const IBlend BLEND_01 = IBlend (EBlend::Default, 0.01f);
};