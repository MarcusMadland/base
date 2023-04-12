#include "../../include/mapp/window.hpp"

#if !defined(MAPP_CUSTOM_PLATFORM_DETECTION)
// @todo Automatic platform detection
#endif

// Include correct header depending on platform
#ifdef MAPP_WIN32
#include "mapp/win32/win32_window.hpp"
#endif

#ifdef MAPP_COCOA
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
		return new WindowWin32(params);
#endif
        
#ifdef MAPP_COCOA
		return new WindowCocoa(params);
#endif
        
        return nullptr;
	}

}
