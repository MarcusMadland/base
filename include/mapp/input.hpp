#pragma once

#ifdef MAPP_PLATFORM_WIN32
#include "win32/win32_input.hpp"
#endif

#ifdef MAPP_PLATFORM_COCOA
#include "cocoa/cocoa_input.hpp"
#endif
