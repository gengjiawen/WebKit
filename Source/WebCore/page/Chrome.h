/*
 * Copyright (C) 2006-2017 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Copyright (C) 2012, Samsung Electronics. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include "Cursor.h"
#include "DisabledAdaptations.h"
#include "FocusDirection.h"
#include "HostWindow.h"
#include <wtf/Forward.h>
#include <wtf/FunctionDispatcher.h>
#include <wtf/RefPtr.h>
#include <wtf/UniqueRef.h>

#if PLATFORM(COCOA)
OBJC_CLASS NSView;
#endif

namespace PAL::WebGPU {
class GPU;
}

namespace WebCore {

enum class TextDirection : bool;

class ChromeClient;
class ColorChooser;
class ColorChooserClient;
class DataListSuggestionPicker;
class DataListSuggestionsClient;
class DateTimeChooser;
class DateTimeChooserClient;
class FileChooser;
class FileIconLoader;
class FloatRect;
class Element;
class Frame;
class Geolocation;
class HitTestResult;
class IntPoint;
class IntRect;
class NavigationAction;
class Page;
class PopupMenu;
class PopupMenuClient;
class PopupOpeningObserver;
class SearchPopupMenu;
class WorkerClient;

struct AppHighlight;
struct ContactInfo;
struct ContactsRequestData;
struct DateTimeChooserParameters;
struct ShareDataWithParsedURL;
struct ViewportArguments;
struct WindowFeatures;

class Chrome : public HostWindow {
public:
    Chrome(Page&, UniqueRef<ChromeClient>&&);
    virtual ~Chrome();

    ChromeClient& client() { return m_client; }

    // HostWindow methods.
    void invalidateRootView(const IntRect&) override;
    void invalidateContentsAndRootView(const IntRect&) override;
    void invalidateContentsForSlowScroll(const IntRect&) override;
    void scroll(const IntSize&, const IntRect&, const IntRect&) override;
    IntPoint screenToRootView(const IntPoint&) const override;
    IntRect rootViewToScreen(const IntRect&) const override;
    IntPoint accessibilityScreenToRootView(const IntPoint&) const override;
    IntRect rootViewToAccessibilityScreen(const IntRect&) const override;
    PlatformPageClient platformPageClient() const override;
    void setCursor(const Cursor&) override;
    void setCursorHiddenUntilMouseMoves(bool) override;

    RefPtr<ImageBuffer> createImageBuffer(const FloatSize&, RenderingMode, RenderingPurpose, float resolutionScale, const DestinationColorSpace&, PixelFormat, bool avoidBackendSizeCheck = false) const override;
    RefPtr<WebCore::ImageBuffer> sinkIntoImageBuffer(std::unique_ptr<WebCore::SerializedImageBuffer>) override;

#if ENABLE(WEBGL)
    RefPtr<GraphicsContextGL> createGraphicsContextGL(const GraphicsContextGLAttributes&) const override;
#endif

    RefPtr<PAL::WebGPU::GPU> createGPUForWebGPU() const;

    PlatformDisplayID displayID() const override;
    void windowScreenDidChange(PlatformDisplayID, std::optional<FramesPerSecond>) override;

    FloatSize screenSize() const override;
    FloatSize availableScreenSize() const override;
    FloatSize overrideScreenSize() const override;

    void scrollContainingScrollViewsToRevealRect(const IntRect&) const;
    void scrollMainFrameToRevealRect(const IntRect&) const;

    void contentsSizeChanged(Frame&, const IntSize&) const;

    WEBCORE_EXPORT void setWindowRect(const FloatRect&);
    WEBCORE_EXPORT FloatRect windowRect() const;

    FloatRect pageRect() const;

    void focus();
    void unfocus();

    bool canTakeFocus(FocusDirection) const;
    void takeFocus(FocusDirection);

    void focusedElementChanged(Element*);
    void focusedFrameChanged(Frame*);

    WEBCORE_EXPORT Page* createWindow(Frame&, const WindowFeatures&, const NavigationAction&);
    WEBCORE_EXPORT void show();

    bool canRunModal() const;
    void runModal();

    void setToolbarsVisible(bool);
    bool toolbarsVisible() const;

    void setStatusbarVisible(bool);
    bool statusbarVisible() const;

    void setScrollbarsVisible(bool);
    bool scrollbarsVisible() const;

    void setMenubarVisible(bool);
    bool menubarVisible() const;

    void setResizable(bool);

    bool canRunBeforeUnloadConfirmPanel();
    bool runBeforeUnloadConfirmPanel(const String& message, Frame&);

    void closeWindow();

    void runJavaScriptAlert(Frame&, const String&);
    bool runJavaScriptConfirm(Frame&, const String&);
    bool runJavaScriptPrompt(Frame&, const String& message, const String& defaultValue, String& result);
    WEBCORE_EXPORT void setStatusbarText(Frame&, const String&);

    void mouseDidMoveOverElement(const HitTestResult&, unsigned modifierFlags);

    WEBCORE_EXPORT bool print(Frame&);

    WEBCORE_EXPORT void enableSuddenTermination();
    WEBCORE_EXPORT void disableSuddenTermination();

#if ENABLE(INPUT_TYPE_COLOR)
    std::unique_ptr<ColorChooser> createColorChooser(ColorChooserClient&, const Color& initialColor);
#endif

#if ENABLE(DATALIST_ELEMENT)
    std::unique_ptr<DataListSuggestionPicker> createDataListSuggestionPicker(DataListSuggestionsClient&);
#endif

#if ENABLE(DATE_AND_TIME_INPUT_TYPES)
    std::unique_ptr<DateTimeChooser> createDateTimeChooser(DateTimeChooserClient&);
#endif

    std::unique_ptr<WorkerClient> createWorkerClient(SerialFunctionDispatcher&);

#if ENABLE(APP_HIGHLIGHTS)
    void storeAppHighlight(AppHighlight&&) const;
#endif

    void runOpenPanel(Frame&, FileChooser&);
    void showShareSheet(ShareDataWithParsedURL&, CompletionHandler<void(bool)>&&);
    void showContactPicker(const ContactsRequestData&, CompletionHandler<void(std::optional<Vector<ContactInfo>>&&)>&&);
    void loadIconForFiles(const Vector<String>&, FileIconLoader&);

    void dispatchDisabledAdaptationsDidChange(const OptionSet<DisabledAdaptations>&) const;
    void dispatchViewportPropertiesDidChange(const ViewportArguments&) const;

    bool requiresFullscreenForVideoPlayback();

#if PLATFORM(COCOA)
    WEBCORE_EXPORT void focusNSView(NSView*);
#endif

    bool selectItemWritingDirectionIsNatural();
    bool selectItemAlignmentFollowsMenuWritingDirection();
    RefPtr<PopupMenu> createPopupMenu(PopupMenuClient&) const;
    RefPtr<SearchPopupMenu> createSearchPopupMenu(PopupMenuClient&) const;

#if PLATFORM(IOS_FAMILY)
    // FIXME: Can we come up with a better name for this setter?
    void setDispatchViewportDataDidChangeSuppressed(bool dispatchViewportDataDidChangeSuppressed) { m_isDispatchViewportDataDidChangeSuppressed = dispatchViewportDataDidChangeSuppressed; }
#endif

    void didReceiveDocType(Frame&);

    void registerPopupOpeningObserver(PopupOpeningObserver&);
    void unregisterPopupOpeningObserver(PopupOpeningObserver&);

private:
    void notifyPopupOpeningObservers() const;

    void getToolTip(const HitTestResult&, String&, TextDirection&);

    Page& m_page;
    UniqueRef<ChromeClient> m_client;
    // FIXME: This should be WeakPtr<PopupOpeningObserver>.
    Vector<PopupOpeningObserver*> m_popupOpeningObservers;
#if PLATFORM(IOS_FAMILY)
    bool m_isDispatchViewportDataDidChangeSuppressed { false };
#endif
};

} // namespace WebCore
