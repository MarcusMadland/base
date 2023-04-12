#ifdef MAPP_COCOA

#pragma once

#include <Cocoa/Cocoa.h>

#include "../include/mapp/window.hpp"

namespace mapp
{
	class WindowCocoa : public Window
	{
	public:
		WindowCocoa(const WindowParams& params);
		~WindowCocoa();

		virtual void onUpdate(const float& dt) override;
		virtual void* getNativeWindow() override;
		virtual void* getNativeDisplay() override;

	private:
		NSWindow* window;
	};
}

#endif