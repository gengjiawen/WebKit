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

#pragma once

#include "ElementIdentifier.h"
#include "FloatRect.h"
#include "Region.h"

namespace IPC {
class Decoder;
class Encoder;
}

namespace WTF {
class TextStream;
}

namespace WebCore {

class Element;
class Page;
class RenderObject;

struct InteractionRegion {
    enum class Type : bool { Interaction, Occlusion };

    ElementIdentifier elementIdentifier;
    Region regionInLayerCoordinates;
    float borderRadius { 0 };
    Type type;

    WEBCORE_EXPORT ~InteractionRegion();
};

inline bool operator==(const InteractionRegion& a, const InteractionRegion& b)
{
    return a.elementIdentifier == b.elementIdentifier
        && a.regionInLayerCoordinates == b.regionInLayerCoordinates
        && a.borderRadius == b.borderRadius
        && a.type == b.type;
}

WEBCORE_EXPORT std::optional<InteractionRegion> interactionRegionForRenderedRegion(RenderObject&, const Region&);

WTF::TextStream& operator<<(WTF::TextStream&, const InteractionRegion&);

}
