/*
 * Copyright (C) 2006-2023 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#if USE(CG)

#include "ColorSpaceCG.h"
#include "GraphicsContext.h"

namespace WebCore {

class WEBCORE_EXPORT GraphicsContextCG : public GraphicsContext {
public:
    enum CGContextSource {
        Unknown,
        CGContextFromCALayer
    };
    GraphicsContextCG(CGContextRef, CGContextSource = CGContextSource::Unknown);

    ~GraphicsContextCG();

    bool hasPlatformContext() const final;
    CGContextRef platformContext() const final;

    const DestinationColorSpace& colorSpace() const final;

    void save() final;
    void restore() final;

    void drawRect(const FloatRect&, float borderThickness = 1) final;
    void drawLine(const FloatPoint&, const FloatPoint&) final;
    void drawEllipse(const FloatRect&) final;

    void applyStrokePattern() final;
    void applyFillPattern() final;
    void drawPath(const Path&) final;
    void fillPath(const Path&) final;
    void strokePath(const Path&) final;

    void beginTransparencyLayer(float opacity) final;
    void endTransparencyLayer() final;

    void applyDeviceScaleFactor(float factor) final;

    using GraphicsContext::fillRect;
    void fillRect(const FloatRect&) final;
    void fillRect(const FloatRect&, const Color&) final;
    void fillRoundedRectImpl(const FloatRoundedRect&, const Color&) final;
    void fillRectWithRoundedHole(const FloatRect&, const FloatRoundedRect& roundedHoleRect, const Color&) final;
    void clearRect(const FloatRect&) final;
    void strokeRect(const FloatRect&, float lineWidth) final;

    void fillEllipse(const FloatRect& ellipse) final;
    void strokeEllipse(const FloatRect& ellipse) final;

    bool isCALayerContext() const final;

    RenderingMode renderingMode() const final;

    void clip(const FloatRect&) final;
    void clipOut(const FloatRect&) final;

    void clipOut(const Path&) final;

    void clipPath(const Path&, WindRule = WindRule::EvenOdd) final;

    void clipToImageBuffer(ImageBuffer&, const FloatRect&) final;

    IntRect clipBounds() const final;

    void setLineCap(LineCap) final;
    void setLineDash(const DashArray&, float dashOffset) final;
    void setLineJoin(LineJoin) final;
    void setMiterLimit(float) final;

    void drawPattern(NativeImage&, const FloatRect& destRect, const FloatRect& tileRect, const AffineTransform& patternTransform, const FloatPoint& phase, const FloatSize& spacing, const ImagePaintingOptions& = { }) final;
    bool needsCachedNativeImageInvalidationWorkaround(RenderingMode) override;

    using GraphicsContext::scale;
    void scale(const FloatSize&) final;
    void rotate(float angleInRadians) final;
    void translate(float x, float y) final;

    void concatCTM(const AffineTransform&) final;
    void setCTM(const AffineTransform&) override;

    AffineTransform getCTM(IncludeDeviceScale = PossiblyIncludeDeviceScale) const override;

    void drawFocusRing(const Path&, float outlineWidth, const Color&) final;
    void drawFocusRing(const Vector<FloatRect>&, float outlineOffset, float outlineWidth, const Color&) final;

    void drawLinesForText(const FloatPoint&, float thickness, const DashArray& widths, bool printing, bool doubleLines, StrokeStyle) final;

    void drawDotsForDocumentMarker(const FloatRect&, DocumentMarkerLineStyle) final;

    void setURLForRect(const URL&, const FloatRect&) final;

    void setDestinationForRect(const String& name, const FloatRect&) final;
    void addDestinationAtPoint(const String& name, const FloatPoint&) final;

    bool supportsInternalLinks() const final;

    void didUpdateState(GraphicsContextState&) final;

    virtual bool canUseShadowBlur() const;

    virtual FloatRect roundToDevicePixels(const FloatRect&, RoundingMode = RoundAllSides) const;

protected:
    virtual void setCGShadow(RenderingMode, const FloatSize& offset, float blur, const Color&, bool shadowsIgnoreTransforms);

private:
    void convertToDestinationColorSpaceIfNeeded(RetainPtr<CGImageRef>&);
    void drawNativeImageInternal(NativeImage&, const FloatSize& selfSize, const FloatRect& destRect, const FloatRect& srcRect, const ImagePaintingOptions& = { }) final;

    void clearCGShadow();

    const RetainPtr<CGContextRef> m_cgContext;
    const RenderingMode m_renderingMode : 1; // NOLINT
    const bool m_isLayerCGContext : 1;
    mutable bool m_userToDeviceTransformKnownToBeIdentity : 1 { false };
    mutable std::optional<DestinationColorSpace> m_colorSpace;
};

CGAffineTransform getUserToBaseCTM(CGContextRef);

} // namespace WebCore

#include "CGContextStateSaver.h"

#endif // USE(CG)
