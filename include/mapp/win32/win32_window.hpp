#ifdef MAPP_WIN32

#pragma once

#include "../include/mapp/window.hpp"

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace mapp
{
	class WindowWin32 : public Window
	{
	public:
		WindowWin32(const WindowParams& params);
		~WindowWin32();

		bool ProcessMessages();

		virtual void onUpdate(const float& dt) override;
		virtual void* getNativeWindow() override;
		virtual void* getNativeDisplay() override;

	private:
		const wchar_t* className;
		HINSTANCE instance;
		HWND window;
	};
}

#endif
