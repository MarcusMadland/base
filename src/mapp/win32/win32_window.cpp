#ifdef MAPP_PLATFORM_WIN32

#include "../include/mapp/win32/win32_window.hpp"
#include "../include/mapp/app.hpp"

#include <windowsx.h>
#include <iostream>
#include <stdlib.h>

namespace mapp
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CLOSE:
			{
				DestroyWindow(hWnd);

				if (App::isValid())
				{
					mapp::WindowCloseEvent event;
					App::getInstance().getWindow()->eventCallback(event);
				}

				break;
			}

			case WM_SIZE:
			{
				SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
				PostMessage(hWnd, WM_PAINT, 0, 0);

				if (App::isValid())
				{
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);

					mapp::WindowResizeEvent event = mapp::WindowResizeEvent(width, height);
					App::getInstance().getWindow()->eventCallback(event);
				}

				break;
			}

			case WM_KEYDOWN:
			{
				if (App::isValid())
				{
					const bool wasDown = (lParam & (1 << 30)) != 0;

					if (!wasDown)
					{
						const uint64_t keyCode = wParam;

						mapp::KeyPressedEvent event = mapp::KeyPressedEvent(keyCode);
						App::getInstance().getWindow()->eventCallback(event);
					}
					else
					{
						const uint64_t keyCode = wParam;

						mapp::KeyPressingEvent event = mapp::KeyPressingEvent(keyCode);
						App::getInstance().getWindow()->eventCallback(event);
					}
				}

				break;
			}

			case WM_KEYUP:
			{
				if (App::isValid())
				{
					const bool wasDown = (lParam & (1 << 30)) != 0;
					const bool isDown = (lParam & (1 << 31)) == 0;

					if (wasDown && !isDown)
					{
						const uint64_t keyCode = wParam;

						mapp::KeyReleasedEvent event = mapp::KeyReleasedEvent(keyCode);
						App::getInstance().getWindow()->eventCallback(event);
					}
				}

				break;
			}

			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			{
				if (App::isValid())
				{
					const uint64_t keyCode = wParam;

					mapp::MouseButtonPressedEvent event = mapp::MouseButtonPressedEvent(keyCode);
					App::getInstance().getWindow()->eventCallback(event);
				}

				break;
			}

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				if (App::isValid())
				{
					const uint64_t keyCode = wParam;

					mapp::MouseButtonReleasedEvent event = mapp::MouseButtonReleasedEvent(keyCode);
					App::getInstance().getWindow()->eventCallback(event);
					
				}

				break;
			}

			case WM_MOUSEMOVE:
			{
				if (App::isValid())
				{
					mapp::MouseMovedEvent event = mapp::MouseMovedEvent(static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)));
					App::getInstance().getWindow()->eventCallback(event);
				}

				break;
			}

			case WM_MOUSEWHEEL:
			{
				if (App::isValid())
				{
					mapp::MouseScrolledEvent event = mapp::MouseScrolledEvent(0.0f, GET_WHEEL_DELTA_WPARAM(wParam));
					App::getInstance().getWindow()->eventCallback(event);
				}

				break;
			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	WindowWin32::WindowWin32(const WindowParams& params)
		: Window(params)
		, className(L"WindowsWindowClass")
	{
		// Class
		instance = GetModuleHandle(nullptr);
		WNDCLASS windowClass = WNDCLASS();
		windowClass.lpszClassName = className;
		windowClass.hInstance = instance;
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpfnWndProc = WindowProc; 
		windowClass.style = (params.canClose ? 0 : CS_NOCLOSE);
		RegisterClass(&windowClass);

		// Size & Style
		DWORD style = 
			WS_CAPTION | WS_SYSMENU |
			(params.canResize ? WS_MAXIMIZEBOX : 0) |
			(params.canResize ? WS_MINIMIZEBOX : 0) |
			(params.canResize ? WS_SIZEBOX : 0);

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
		if (!params.showBorder)
		{
			SetWindowLong(window, GWL_STYLE, 0);
		}
		ShowWindow(window, SW_SHOW);
	}

	WindowWin32::~WindowWin32()
	{
		UnregisterClass(className, instance);
	}

	void WindowWin32::onUpdate(const float& dt)
	{
		MSG msg = MSG();

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	bool WindowWin32::setFullscreen(const bool enable)
	{
		bool isChangeSuccessful;

		if (enable)
		{
			HDC windowHDC = GetDC(window);
			int fullscreenWidth = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
			int fullscreenHeight = GetDeviceCaps(windowHDC, DESKTOPVERTRES);
			int colourBits = GetDeviceCaps(windowHDC, BITSPIXEL);
			int refreshRate = GetDeviceCaps(windowHDC, VREFRESH);

			DEVMODE fullscreenSettings = DEVMODE();

			EnumDisplaySettings(NULL, 0, &fullscreenSettings);
			fullscreenSettings.dmPelsWidth = fullscreenWidth;
			fullscreenSettings.dmPelsHeight = fullscreenHeight;
			fullscreenSettings.dmBitsPerPel = colourBits;
			fullscreenSettings.dmDisplayFrequency = refreshRate;
			fullscreenSettings.dmFields = DM_PELSWIDTH |
				DM_PELSHEIGHT |
				DM_BITSPERPEL |
				DM_DISPLAYFREQUENCY;

			SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
			SetWindowLongPtr(window, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(window, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
			isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
			ShowWindow(window, SW_MAXIMIZE);

			return isChangeSuccessful;
		}
		else
		{
			SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_LEFT);
			SetWindowLongPtr(window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
			SetWindowPos(window, HWND_NOTOPMOST, params.width, params.height, params.width + 0, params.height + 0, SWP_SHOWWINDOW);
			ShowWindow(window, SW_RESTORE);

			return isChangeSuccessful;
		}
	}

	void* WindowWin32::getNativeWindow()
	{
		return window;
	}

	void* WindowWin32::getNativeDisplay()
	{
		return nullptr;
	}
}

#endif
