
#include "../../include/mapp/window.hpp"

namespace mapp
{
	Window::Window(const char* name, uint32_t width, uint32_t height)
	{
		windowInfo.title = name;
		windowInfo.width = width;
		windowInfo.height = height;

		onInit();
	}

	Window::~Window()
	{
		onShutdown();
	}

	void Window::setEventCallback(const std::function<void(Event&)>& callback)
	{
		windowInfo.eventCallback = callback;
	}
}
