#include "../include/mapp/cocoa/cocoa_window.hpp"

#include <iostream>
#include <stdlib.h>

namespace mapp
{
	WindowMac::WindowMac(const WindowParams& params)
		: Window(params)
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
