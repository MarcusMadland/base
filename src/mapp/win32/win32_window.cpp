#include "../include/mapp/platform.hpp"

#ifdef MAPP_PLATFORM_WIN32

#include "../include/mapp/win32/win32_window.hpp"
#include "../include/mapp/app.hpp"

#include <windowsx.h>
#include <Xinput.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

// @todo Should be parameters for the user right?
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30
#define INPUT_DEADZONE						8000

namespace mapp
{
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CREATE:
			{
				

				break;
			}
			

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

			// Keyboard
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

			// Mouse
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
					float inMin = -120.0f;
					float inMax = 120.0f;
					float outMin = -1.0f;
					float outMax = 1.0f;

					// Map the input values so they will be from -1 to 1 instead of inMin to inMax
					float x = ((float)GET_WHEEL_DELTA_WPARAM(wParam) - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

					mapp::MouseScrolledEvent event = mapp::MouseScrolledEvent(x);
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

	std::string GetLastErrorAsString()
	{
		//Get the error message ID, if any.
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0) {
			return std::string(); //No error message has been recorded
		}

		LPSTR messageBuffer = nullptr;

		//Ask Win32 to give us the string version of that message ID.
		//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		//Copy the error message into a std::string.
		std::string message(messageBuffer, size);

		//Free the Win32's string's buffer.
		LocalFree(messageBuffer);

		return message;
	}

	WindowWin32::WindowWin32(const WindowParams& params)
		: Window(params)
		, className(L"WindowsWindowClass")
	{
		// Instance
		instance = GetModuleHandle(0);

		// Class
		WNDCLASSEX windowClass = WNDCLASSEX();
		windowClass.lpszClassName = className;
		windowClass.hInstance = instance;
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = WS_EX_NOPARENTNOTIFY;
		windowClass.lpfnWndProc = WindowProc; 
		windowClass.style = CS_HREDRAW | CS_VREDRAW | (params.canClose ? 0 : CS_NOCLOSE);
		windowClass.hIcon = NULL;
		windowClass.hIconSm = NULL;
		
		if (!RegisterClassEx(&windowClass))
		{
			std::cout << GetLastErrorAsString().c_str() << std::endl;
		}
		
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
		const size_t len = params.title.length() + 1;
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

		// Gamepad
		DWORD dwResult;
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			// Simply get the state of the controller from XInput.
			dwResult = XInputGetState(i, &state);

			if (dwResult == ERROR_SUCCESS)
			{
				// Input Joysticks
				if (App::isValid())
				{
					float inMin = -32768.0f;
					float inMax = 32768.0f;
					float outMin = -1.0f;
					float outMax = 1.0f;

					// Map the input values so they will be from -1 to 1 instead of inMin to inMax
					float x = ((float)state.Gamepad.sThumbLX - inMin)* (outMax - outMin) / (inMax - inMin) + outMin;
					float y = ((float)state.Gamepad.sThumbLY - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

					// Check for deadzone with the nonmapped value
					if (abs((float)state.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
						abs((float)state.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					{
						mapp::GamepadLeftJoystickEvent event = mapp::GamepadLeftJoystickEvent(x, y, i);
						App::getInstance().getWindow()->eventCallback(event);
					}

					x = y = 0;
					x = ((float)state.Gamepad.sThumbRX - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
					y = ((float)state.Gamepad.sThumbRY - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

					if (abs((float)state.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || 
						abs((float)state.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
					{
						mapp::GamepadRightJoystickEvent event = mapp::GamepadRightJoystickEvent(x, y, i);
						App::getInstance().getWindow()->eventCallback(event);
					}
					
				}

				// Input Triggers
				if (App::isValid())
				{
					float inMin = -255.0f;
					float inMax = 255.0f;
					float outMin = -1.0f;
					float outMax = 1.0f;

					// Map the input values so they will be from -1 to 1 instead of inMin to inMax
					float x = ((float)state.Gamepad.bLeftTrigger - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
					
					// Check for deadzone with the nonmapped value
					if (abs((float)state.Gamepad.bLeftTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
					{
						mapp::GamepadLeftTriggerEvent event = mapp::GamepadLeftTriggerEvent(x, i);
						App::getInstance().getWindow()->eventCallback(event);
					}

					x = 0;
					x = ((float)state.Gamepad.bRightTrigger - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	

					if (abs((float)state.Gamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
					{
						mapp::GamepadRightTriggerEvent event = mapp::GamepadRightTriggerEvent(x, i);
						App::getInstance().getWindow()->eventCallback(event);
					}

				}

				// Input Buttons
				static bool wasDown = false;
				static uint64_t keyCode = 0;
				if (state.Gamepad.wButtons != 0)
				{
					if (wasDown)
					{
						if (App::isValid())
						{
							keyCode = state.Gamepad.wButtons;

							mapp::GamepadKeyPressingEvent event = mapp::GamepadKeyPressingEvent(keyCode, i);
							App::getInstance().getWindow()->eventCallback(event);
						}
					}
					else
					{
						if (App::isValid())
						{
							wasDown = true;

							keyCode = state.Gamepad.wButtons;

							mapp::GamepadKeyPressedEvent event = mapp::GamepadKeyPressedEvent(keyCode, i);
							App::getInstance().getWindow()->eventCallback(event);
						}
					}
				}
				else if (wasDown)
				{
					if (App::isValid())
					{
						wasDown = false;

						mapp::GamepadKeyReleasedEvent event = mapp::GamepadKeyReleasedEvent(keyCode, i);
						App::getInstance().getWindow()->eventCallback(event);
					}
				}
			}
		}
	}

	bool WindowWin32::setFullscreen(const bool enable)
	{
		bool isChangeSuccessful;

		HDC windowHDC = GetDC(window);
		int fullscreenWidth = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
		int fullscreenHeight = GetDeviceCaps(windowHDC, DESKTOPVERTRES);

		// Fullscreen
		if (enable)
		{
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

			// Resize event
			if (App::isValid())
			{
				mapp::WindowResizeEvent event = mapp::WindowResizeEvent(fullscreenWidth, fullscreenHeight);
				App::getInstance().getWindow()->eventCallback(event);
			}
			else
			{
				// @todo Error, do not set screen to fullscreen before application is created
			}
		}
		else
		{
			SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_LEFT);
			SetWindowLongPtr(window, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
			SetWindowPos(window, HWND_NOTOPMOST, params.width, params.height, params.width + 0, params.height + 0, SWP_SHOWWINDOW);
			ShowWindow(window, SW_RESTORE);
		}

		return isChangeSuccessful;
	}

	bool WindowWin32::isGamepadConnected()
	{

		return false;
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
