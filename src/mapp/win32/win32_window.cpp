#include "../include/mapp/win32/win32_window.hpp"

#include <iostream>
#include <stdlib.h>

namespace mapp
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0 ;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	WindowWin::WindowWin(const WindowParams& params)
		: Window(params)
		, className(L"WindowsWindowClass")
	{
		// Class
		instance = GetModuleHandle(nullptr);
		WNDCLASS windowClass = WNDCLASS();
		windowClass.lpszClassName = className;
		windowClass.hInstance = instance;
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpfnWndProc = WindowProc; // @todo fix later
		RegisterClass(&windowClass);

		// Size & Style
		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		RECT rect = RECT();
		rect.left = 250;
		rect.top = 250;
		rect.right = rect.left + params.width;
		rect.bottom = rect.top + params.height;
		AdjustWindowRect(&rect, style, false);

		// Window
		const size_t len = std::strlen(params.title.c_str()) + 1;
		wchar_t* windowTitle = new wchar_t[len];
		std::mbstowcs(windowTitle, params.title.c_str(), len);
		window = CreateWindowEx(
			0,
			className,
			windowTitle,
			style,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			nullptr,
			nullptr,
			instance,
			nullptr
		);
		ShowWindow(window, SW_SHOW);
	}

	WindowWin::~WindowWin()
	{
		UnregisterClass(className, instance);
	}

	bool WindowWin::ProcessMessages()
	{
		MSG msg = MSG();

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message = WM_QUIT)
			{
				return false;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return true;
	}

	void WindowWin::onUpdate(const float& dt)
	{
	}

	void* WindowWin::getNativeWindow()
	{
		return window;
	}

	void* WindowWin::getNativeDisplay()
	{
		return nullptr;
	}
}
