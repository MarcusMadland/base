#pragma once

#include "../include/mapp/window.hpp"

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace mapp
{
	class WindowWin : public Window
	{
	public:
		WindowWin(const char* name, uint32_t width, uint32_t height);
		~WindowWin();

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
