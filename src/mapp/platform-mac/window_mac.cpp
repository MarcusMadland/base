#include "../include/mapp/platform-mac/window_mac.hpp"

#include <iostream>
#include <stdlib.h>

namespace mapp
{
	WindowMac::WindowMac(const char* name, uint32_t width, uint32_t height)
		: Window(name, width, height)
	{
		
	}

	WindowMac::~WindowMac()
	{
	}
	
	void WindowMac::onUpdate(const float& dt)
	{
	}

	void* WindowMac::getNativeWindow()
	{
		return nullptr;
	}

	void* WindowMac::getNativeDisplay()
	{
		return nullptr;
	}
}
