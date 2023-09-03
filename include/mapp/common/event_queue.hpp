#pragma once

#include <vector>

#include "event.hpp"

#ifdef XWIN_WIN32
#include "../Win32/WIN32_event_queue.hpp"
#elif XWIN_UWP
#include "../UWP/UWPEventQueue.h"
#elif XWIN_COCOA
#include "../cocoa/CocoaEventQueue.h"
#elif XWIN_XCB
#include "../XCB/XCBEventQueue.h"
#elif XWIN_XLIB
#include "../XLib/XLibEventQueue.h"
#elif XWIN_ANDROID
#include "../Android/AndroidEventQueue.h"
#elif XWIN_UIKIT
#include "../uikit/UIKitEventQueue.h"
#elif XWIN_WASM
#include "../WASM/WASMEventQueue.h"
#elif XWIN_NOOP
#include "../Noop/NoopEventQueue.h"
#endif
