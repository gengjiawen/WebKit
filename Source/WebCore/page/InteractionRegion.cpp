/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "InteractionRegion.h"

#include "Document.h"
#include "ElementAncestorIteratorInlines.h"
#include "ElementInlines.h"
#include "Frame.h"
#include "FrameSnapshotting.h"
#include "FrameView.h"
#include "GeometryUtilities.h"
#include "HTMLAnchorElement.h"
#include "HTMLAttachmentElement.h"
#include "HTMLButtonElement.h"
#include "HTMLFieldSetElement.h"
#include "HTMLFormControlElement.h"
#include "HTMLInputElement.h"
#include "HitTestResult.h"
#include "Page.h"
#include "PathUtilities.h"
#include "PlatformMouseEvent.h"
#include "RenderBox.h"
#include "RenderLayer.h"
#include "RenderLayerBacking.h"
#include "SimpleRange.h"
#include "SliderThumbElement.h"
#include <wtf/NeverDestroyed.h>

namespace WebCore {

InteractionRegion::~InteractionRegion() = default;

static CursorType cursorTypeForElement(Element& element)
{
    auto* renderer = element.renderer();
    auto* style = renderer ? &renderer->style() : nullptr;
    auto cursorType = style ? style->cursor() : CursorType::Auto;

    if (cursorType == CursorType::Auto && element.enclosingLinkEventParentOrSelf())
        cursorType = CursorType::Pointer;

    return cursorType;
}

static bool shouldAllowElement(const Element& element)
{
    if (is<HTMLFieldSetElement>(element))
        return false;

    if (auto* input = dynamicDowncast<HTMLInputElement>(element)) {
        // Do not allow regions for the <input type='range'>, because we make one for the thumb.
        if (input->isRangeControl())
            return false;

        // Do not allow regions for the <input type='file'>, because we make one for the button.
        if (input->isFileUpload())
            return false;
    }

    return true;
}

static bool shouldAllowAccessibilityRoleAsPointerCursorReplacement(const Element& element)
{
    switch (AccessibilityObject::ariaRoleToWebCoreRole(element.attributeWithoutSynchronization(HTMLNames::roleAttr))) {
    case AccessibilityRole::Button:
    case AccessibilityRole::CheckBox:
    case AccessibilityRole::DisclosureTriangle:
    case AccessibilityRole::ImageMapLink:
    case AccessibilityRole::Link:
    case AccessibilityRole::WebCoreLink:
    case AccessibilityRole::ListBoxOption:
    case AccessibilityRole::MenuButton:
    case AccessibilityRole::MenuItem:
    case AccessibilityRole::MenuItemCheckbox:
    case AccessibilityRole::MenuItemRadio:
    case AccessibilityRole::PopUpButton:
    case AccessibilityRole::RadioButton:
    case AccessibilityRole::ToggleButton:
        return true;
    default:
        return false;
    }
}

static bool shouldAllowNonPointerCursorForElement(const Element& element)
{
#if ENABLE(ATTACHMENT_ELEMENT)
    if (is<HTMLAttachmentElement>(element))
        return true;
#endif

    if (is<HTMLFormControlElement>(element))
        return true;

    if (is<SliderThumbElement>(element))
        return true;

    if (shouldAllowAccessibilityRoleAsPointerCursorReplacement(element))
        return true;

    return false;
}

std::optional<InteractionRegion> interactionRegionForRenderedRegion(RenderObject& regionRenderer, const Region& region)
{
    if (!regionRenderer.node())
        return std::nullopt;

    auto bounds = region.bounds();
    if (bounds.isEmpty())
        return std::nullopt;

    auto* localFrame = dynamicDowncast<LocalFrame>(regionRenderer.document().frame()->mainFrame());
    if (!localFrame)
        return std::nullopt;

    auto& mainFrameView = *localFrame->view();

    FloatSize frameViewSize = mainFrameView.size();
    // Adding some wiggle room, we use this to avoid extreme cases.
    auto scale = 1 / mainFrameView.visibleContentScaleFactor() + 0.2;
    frameViewSize.scale(scale, scale);
    auto frameViewArea = frameViewSize.area();

    auto checkedRegionArea = bounds.area<RecordOverflow>();
    if (checkedRegionArea.hasOverflowed())
        return std::nullopt;

    auto originalElement = dynamicDowncast<Element>(regionRenderer.node());
    if (originalElement && originalElement->isPseudoElement())
        return std::nullopt;

    auto matchedElement = originalElement;
    if (!matchedElement)
        matchedElement = regionRenderer.node()->parentElement();
    if (!matchedElement)
        return std::nullopt;

    if (auto* linkElement = matchedElement->enclosingLinkEventParentOrSelf())
        matchedElement = linkElement;
    if (auto* buttonElement = ancestorsOfType<HTMLButtonElement>(*matchedElement).first())
        matchedElement = buttonElement;

    if (!shouldAllowElement(*matchedElement))
        return std::nullopt;

    if (!matchedElement->renderer())
        return std::nullopt;
    auto& renderer = *matchedElement->renderer();

    if (renderer.style().effectivePointerEvents() == PointerEvents::None)
        return std::nullopt;

    bool isOriginalMatch = matchedElement == originalElement;

    // FIXME: Consider also allowing elements that only receive touch events.
    bool hasListener = renderer.style().eventListenerRegionTypes().contains(EventListenerRegionType::MouseClick);
    bool hasPointer = cursorTypeForElement(*matchedElement) == CursorType::Pointer || shouldAllowNonPointerCursorForElement(*matchedElement);
    bool isTooBigForInteraction = checkedRegionArea.value() > frameViewArea / 2;
    if (!hasListener || !hasPointer || isTooBigForInteraction) {
        bool isOverlay = checkedRegionArea.value() <= frameViewArea && (renderer.style().specifiedZIndex() > 0 || renderer.isFixedPositioned());
        if (isOverlay && isOriginalMatch) {
            Region boundsRegion;
            boundsRegion.unite(bounds);

            return { {
                matchedElement->identifier(),
                boundsRegion,
                0,
                InteractionRegion::Type::Occlusion
            } };
        }

        return std::nullopt;
    }

    bool isInlineNonBlock = renderer.isInline() && !renderer.isReplacedOrInlineBlock();

    // The parent will get its own InteractionRegion.
    if (!isOriginalMatch && !isInlineNonBlock)
        return std::nullopt;

    if (isInlineNonBlock)
        bounds.inflate(regionRenderer.document().settings().interactionRegionInlinePadding());

    float borderRadius = 0;
    if (auto* renderBox = dynamicDowncast<RenderBox>(renderer)) {
        borderRadius = renderBox->borderRadii().minimumRadius();

        auto* input = dynamicDowncast<HTMLInputElement>(matchedElement);
        if (input && input->containerElement()) {
            auto borderBoxRect = renderBox->borderBoxRect();
            auto contentBoxRect = renderBox->contentBoxRect();
            bounds.move(IntSize(borderBoxRect.location() - contentBoxRect.location()));
            bounds.expand(IntSize(borderBoxRect.size() - contentBoxRect.size()));
        }
    }

    Region boundsRegion;
    boundsRegion.unite(bounds);

    return { {
        matchedElement->identifier(),
        boundsRegion,
        borderRadius,
        InteractionRegion::Type::Interaction
    } };
}

TextStream& operator<<(TextStream& ts, const InteractionRegion& interactionRegion)
{
    ts.dumpProperty(interactionRegion.type == InteractionRegion::Type::Occlusion ? "occlusion" : "interaction", interactionRegion.regionInLayerCoordinates);
    ts.dumpProperty("borderRadius", interactionRegion.borderRadius);

    return ts;
}

}
