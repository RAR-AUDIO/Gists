#pragma once

#include "IControls.h"
#include "IGraphicsSkia.h"

#include <SkBlurTypes.h>
#include <SkMaskFilter.h>
#include <SkPerlinNoiseShader.h>


class NoiseShaderControl : public iplug::igraphics::IControl
{
  public:
    NoiseShaderControl(const iplug::igraphics::IRECT& bounds, bool isOverlay = false,
                       float opacity = 1.0F)
        : IControl(bounds), mIsOverlay_(isOverlay), mOpacity_(opacity)
    {
        // Create the Perlin Noise shader
        mNoiseShader_ =
            SkShaders::MakeTurbulence(0.5F,                      // Base frequency X
                                      0.5F,                      // Base frequency Y
                                      3,                         // Number of octaves
                                      static_cast<float>(rand()) // Seed value for randomness
            );

        // Configure paint object
        mPaint_.setShader(mNoiseShader_);

        if (mIsOverlay_)
        {
            mPaint_.setAlphaf(mOpacity_);                // Adjust transparency for overlay
            mPaint_.setBlendMode(SkBlendMode::kOverlay); // Overlay blending mode
        }
    }

    void Draw(iplug::igraphics::IGraphics& g) override
    {
        if (g.GetDrawContext()) // Ensure Skia backend
        {
            SkCanvas* canvas = static_cast<SkCanvas*>(g.GetDrawContext());
            SkRect skRect = SkRect::MakeLTRB(mRECT.L, mRECT.T, mRECT.R, mRECT.B);

            // Draw the noise shader over the specified rectangle
            canvas->drawRect(skRect, mPaint_);
        }
    }

  private:
    SkPaint mPaint_;               // Skia paint object to configure drawing
    sk_sp<SkShader> mNoiseShader_; // Perlin noise shader
    bool mIsOverlay_;              // Flag for overlay mode
    float mOpacity_;               // Opacity for transparency control
};

class StaticScratchesControl : public iplug::igraphics::IControl
{
  public:
    StaticScratchesControl(const iplug::igraphics::IRECT& bounds, bool isOverlay = false,
                           float opacity = 1.0F)
        : IControl(bounds), mIsOverlay_(isOverlay), mOpacity_(opacity)
    {
        // Define image dimensions
        int width = static_cast<int>(bounds.W());
        int height = static_cast<int>(bounds.H());

        // Create SkSurface with no alpha blending issues
        auto surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height));

        if (surface)
        {
            SkCanvas* canvas = surface->getCanvas();

            // Clear the canvas to fully transparent
            canvas->clear(SK_ColorTRANSPARENT);

            // Configure scratch paint (force fully opaque white)
            mScratchPaint_.setColor(SkColorSetARGB(255, 255, 255, 255)); // Solid white
            mScratchPaint_.setStrokeWidth(0.5F);
            mScratchPaint_.setAntiAlias(true);
            mScratchPaint_.setAlphaf(8.0F / 100.0F); // 8% Opacity

            // Draw scratches
            DrawScratches(canvas, static_cast<float>(width), static_cast<float>(height),
                          static_cast<int>(1e3)); // 1k scratches

            // Save the result as an immutable SkImage
            mScratchImage_ = surface->makeImageSnapshot();
        }

        // Configure paint for rendering cached scratches
        mPaint_.setBlendMode(SkBlendMode::kSrcOver); // Force normal paint mode
    }

    void Draw(iplug::igraphics::IGraphics& g) override
    {
        if (g.GetDrawContext() && mScratchImage_) // Ensure Skia backend and image exists
        {
            SkCanvas* canvas = static_cast<SkCanvas*>(g.GetDrawContext());
            SkRect skRect = SkRect::MakeLTRB(mRECT.L, mRECT.T, mRECT.R, mRECT.B);

            // Draw the cached scratches image (directly without blending issues)
            canvas->drawImageRect(mScratchImage_, skRect, SkSamplingOptions(), &mPaint_);
        }
    }

  private:
    void DrawScratches(SkCanvas* canvas, float width, float height, int numScratches)
    {
        for (int i = 0; i < numScratches; ++i)
        {
            // Random starting point
            float x1 = static_cast<float>(rand() % static_cast<int>(width));
            float y1 = static_cast<float>(rand() % static_cast<int>(height));

            // Random scratch length
            float length = 5.0F + static_cast<float>(rand() % 20);

            // Random angle (0� to 360�)
            float angle = static_cast<float>(rand() % 360);
            float radian = angle * (static_cast<float>(M_PI) / 180.0F);

            // Calculate end points
            float x2 = x1 + length * cos(radian);
            float y2 = y1 + length * sin(radian);

            // Draw scratch
            canvas->drawLine(x1, y1, x2, y2, mScratchPaint_);
        }
    }

    SkPaint mPaint_;               // Paint object for final drawing of cached image
    sk_sp<SkImage> mScratchImage_; // Cached scratches image
    SkPaint mScratchPaint_;        // Paint object for rendering scratches
    bool mIsOverlay_;              // Flag for overlay mode
    float mOpacity_;               // Opacity for transparency control
};