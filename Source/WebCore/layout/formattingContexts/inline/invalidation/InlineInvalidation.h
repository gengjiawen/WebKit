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

#include <optional>
#include <wtf/Forward.h>

namespace WebCore {

class RenderStyle;

namespace InlineDisplay {
struct Box;
}

namespace Layout {

class Box;
class InlineDamage;
class InlineTextBox;
class InlineFormattingState;

class InlineInvalidation {
public:
    // FIXME: InlineFormattingState should be able to provide all the content for invalidation (i.e. omit any display content).
    InlineInvalidation(InlineDamage&, const InlineFormattingState&, const Vector<InlineDisplay::Box>&);

    void styleChanged(const Box&, const RenderStyle& oldStyle);

    void textInserted(const InlineTextBox* damagedInlineTextBox = nullptr, std::optional<size_t> offset = { });
    void textWillBeRemoved(const InlineTextBox&, std::optional<size_t> offset = { });
    void textWillBeRemoved(UniqueRef<Box>&&);

    void inlineLevelBoxInserted(const Box&);
    void inlineLevelBoxWillBeRemoved(UniqueRef<Box>&&);

    void horizontalConstraintChanged();

private:
    InlineDamage& m_inlineDamage;
    const InlineFormattingState& m_inlineFormattingState;
    const Vector<InlineDisplay::Box>& m_displayBoxes;
};

}
}
