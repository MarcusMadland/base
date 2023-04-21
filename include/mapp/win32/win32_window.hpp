#ifdef MAPP_PLATFORM_WIN32

#pragma once

#include <Windows.h>

#include "../include/mapp/window.hpp"

namespace mapp
{
	class WindowWin32 : public Window
	{
	public:
		WindowWin32(const WindowParams& params);
		~WindowWin32();

		virtual void onUpdate(const float& dt) override;

		virtual bool setFullscreen(const bool enable) override;
		virtual bool isGamepadConnected() override;

		virtual void* getNativeWindow() override;
		virtual void* getNativeDisplay() override;

	private:
		const wchar_t* className;
		HINSTANCE instance;
		HWND window;
	};
}

#endif
