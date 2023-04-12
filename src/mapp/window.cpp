#include "../../include/mapp/window.hpp"

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define MAPP_WIN32
#ifdef _WIN64
	//
#else
	//
#endif

#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
	//
#elif TARGET_OS_MACCATALYST
	#define MAPP_COCOA
#elif TARGET_OS_IPHONE
	//
#elif TARGET_OS_MAC
	//
#else
#   error "Unknown Apple platform"
#endif

#elif __ANDROID__
	//
#elif __linux__
	//
#elif __unix__ 
	//
#elif defined(_POSIX_VERSION)
	//
#else
#   error "Unknown compiler"
#endif

// Include correct header depending on platform
#ifdef MAPP_WIN32
#include "mapp/win32/win32_window.hpp"

#elif MAPP_COCOA
#include "mapp/cocoa/cocoa_window.hpp"

#endif

namespace mapp
{
	Window::Window(const WindowParams& params)
		: params(params)
	{
	}

	Window::~Window()
	{
	}

	void Window::setEventCallback(const std::function<void(Event&)>& callback)
	{
		eventCallback = callback;
    }

	Window* Window::create(const WindowParams& params)
	{
		// Create window depending on platform
#ifdef MAPP_WIN32
		return new WindowWin(params);
#elif MAPP_COCOA
		return new WindowMac(params);
#endif
	}

}
