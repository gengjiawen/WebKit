/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#pragma once

#include "InlineLineTypes.h"
#include <wtf/IsoMallocInlines.h>

namespace WebCore {
namespace Layout {

class InlineInvalidation;

class InlineDamage {
    WTF_MAKE_ISO_ALLOCATED_INLINE(InlineDamage);
public:
    InlineDamage() = default;

    enum class Type {
        // Can't decide the type of damage. Let's nuke all the things.
        Invalid,
        // Content changed or some style property that drives soft wrap opportunities (e.g. going from white-space: pre to normal).
        // This tells us to re-process the inline content and run line layout.
        NeedsContentUpdateAndLineLayout,
        // Same content but either the constraint or some style that may affect line breaking changed (e.g. font-size or containing block set new horizontal constraint). 
        NeedsLineLayout,
        // Line breaking positions are the same, only height related style changed (e.g img's height changes).
        NeedsVerticalAdjustment,
        // Line breaking positions are the same, runs may show up at a different horizontal position (e.g. text-align changes).
        NeedsHorizontalAdjustment 
    };
    Type type() const { return m_damageType; }
    // FIXME: Add support for damage range with multiple, different damage types.
    struct ContentPosition {
        size_t lineIndex { 0 };
        InlineItemPosition inlineItemPosition { };
    };
    std::optional<ContentPosition> contentPosition() const { return m_contentPosition; }

    void addDetachedBox(UniqueRef<Box>&& layoutBox) { m_detachedLayoutBoxes.append(WTFMove(layoutBox)); }

private:
    friend class InlineInvalidation;

    void setDamageType(Type type) { m_damageType = type; }
    void setDamagedPosition(ContentPosition contentPosition) { m_contentPosition = contentPosition; }

    Type m_damageType { Type::Invalid };
    std::optional<ContentPosition> m_contentPosition;
    Vector<UniqueRef<Box>> m_detachedLayoutBoxes;
};

}
}
