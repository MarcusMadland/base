/*
 * Copyright 2022 Marcus Madland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mapp/platform.hpp"

#ifdef MAPP_PLATFORM_WIN32

#include "mapp/win32/win32_window.hpp"
#include "mapp/app.hpp"

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

namespace mapp {

	static bool doOnce = true;

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Retrieve the pointer to your window wrapper class instance
		WindowWin32* pWindowWrapper = nullptr;
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			pWindowWrapper = static_cast<WindowWin32*>(pCreateStruct->lpCreateParams);

			// Associate the pointer to the window's user data
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindowWrapper));
		}
		else
		{
			// Retrieve the pointer from the window's user data
			pWindowWrapper = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		}

		if (pWindowWrapper && pWindowWrapper->getNativeWindow())
		{

			switch (uMsg)
			{
			case WM_CLOSE:
			{
				DestroyWindow(hWnd);

				mapp::WindowCloseEvent event;
				pWindowWrapper->mEventCallback(event);

				break;
			}

			case WM_SIZE:
			{
				SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
				PostMessage(hWnd, WM_PAINT, 0, 0);

				// We make sure we never resize the first time, this will give a crash in the win32 api @todo make better
				if (!doOnce)
				{
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);

					mapp::WindowResizeEvent event = mapp::WindowResizeEvent(width, height);
					pWindowWrapper->mEventCallback(event);
				}
				doOnce = false;

				break;
			}

			// Keyboard
			case WM_KEYDOWN:
			{
				const bool wasDown = (lParam & (1 << 30)) != 0;

				if (!wasDown)
				{
					const uint64_t keyCode = wParam;

					mapp::KeyPressedEvent event = mapp::KeyPressedEvent(keyCode);
					pWindowWrapper->mEventCallback(event);
				}
				else
				{
					const uint64_t keyCode = wParam;

					mapp::KeyPressingEvent event = mapp::KeyPressingEvent(keyCode);
					pWindowWrapper->mEventCallback(event);
				}

				break;
			}

			case WM_KEYUP:
			{
				const bool wasDown = (lParam & (1 << 30)) != 0;
				const bool isDown = (lParam & (1 << 31)) == 0;

				if (wasDown && !isDown)
				{
					const uint64_t keyCode = wParam;

					mapp::KeyReleasedEvent event = mapp::KeyReleasedEvent(keyCode);
					pWindowWrapper->mEventCallback(event);
				}

				break;
			}

			// Mouse
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			{
				const uint64_t keyCode = wParam;

				mapp::MouseButtonPressedEvent event = mapp::MouseButtonPressedEvent(keyCode);
				pWindowWrapper->mEventCallback(event);

				break;
			}

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			{
				const uint64_t keyCode = wParam;

				mapp::MouseButtonReleasedEvent event = mapp::MouseButtonReleasedEvent(keyCode);
				pWindowWrapper->mEventCallback(event);

				break;
			}

			case WM_MOUSEMOVE:
			{
				mapp::MouseMovedEvent event = mapp::MouseMovedEvent(static_cast<float>(GET_X_LPARAM(lParam)), static_cast<float>(GET_Y_LPARAM(lParam)));
				pWindowWrapper->mEventCallback(event);

				break;
			}

			case WM_MOUSEWHEEL:
			{
				float inMin = -120.0f;
				float inMax = 120.0f;
				float outMin = -1.0f;
				float outMax = 1.0f;

				// Map the input values so they will be from -1 to 1 instead of inMin to inMax
				float x = ((float)GET_WHEEL_DELTA_WPARAM(wParam) - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

				mapp::MouseScrolledEvent event = mapp::MouseScrolledEvent(x);
				pWindowWrapper->mEventCallback(event);

				break;
			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
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
	, mClassName(L"WindowsWindowClass")
	, mIsFullscreen(false)
{
	// Instance
	mInstance = GetModuleHandle(0);

	// Class
	WNDCLASSEX windowClass = WNDCLASSEX();
	windowClass.lpszClassName = mClassName;
	windowClass.hInstance = mInstance;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = WS_EX_NOPARENTNOTIFY;
	windowClass.lpfnWndProc = WindowProc; 
	windowClass.style = CS_HREDRAW | CS_VREDRAW | (params.mCanClose ? 0 : CS_NOCLOSE);
	windowClass.hIcon = NULL;
	windowClass.hIconSm = NULL;
		
	if (!RegisterClassEx(&windowClass))
	{
		std::cout << GetLastErrorAsString().c_str() << std::endl;
	}
		
	// Size & Style
	DWORD style = 
		WS_CAPTION | WS_SYSMENU |
		(params.mCanResize ? WS_MAXIMIZEBOX : 0) |
		(params.mCanResize ? WS_MINIMIZEBOX : 0) |
		(params.mCanResize ? WS_SIZEBOX : 0);

	RECT rect = RECT();
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + params.mWidth;
	rect.bottom = rect.top + params.mHeight;
	AdjustWindowRect(&rect, style, false);

	// Window
	const size_t len = params.mTitle.length() + 1;
	wchar_t* windowTitle = new wchar_t[len];
	std::mbstowcs(windowTitle, params.mTitle.c_str(), len);
	mWindow = CreateWindowEx(
		0,
		mClassName,
		windowTitle,
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		mInstance,
		reinterpret_cast<LPVOID>(this)
	);
	if (!params.mShowBorder)
	{
		SetWindowLong(mWindow, GWL_STYLE, 0);
	}

	ShowWindow(mWindow, SW_SHOW);
	UpdateWindow(mWindow);
}

WindowWin32::~WindowWin32()
{
	UnregisterClass(mClassName, mInstance);
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
					mEventCallback(event);
				}

				x = y = 0;
				x = ((float)state.Gamepad.sThumbRX - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
				y = ((float)state.Gamepad.sThumbRY - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;

				if (abs((float)state.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || 
					abs((float)state.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
				{
					mapp::GamepadRightJoystickEvent event = mapp::GamepadRightJoystickEvent(x, y, i);
					mEventCallback(event);
				}
					
			}

			// Input Triggers
				
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
					mEventCallback(event);
				}

				x = 0;
				x = ((float)state.Gamepad.bRightTrigger - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	

				if (abs((float)state.Gamepad.bRightTrigger) > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
				{
					mapp::GamepadRightTriggerEvent event = mapp::GamepadRightTriggerEvent(x, i);
					mEventCallback(event);
				}

			}

			// Input Buttons
			static bool wasDown = false;
			static uint64_t keyCode = 0;
			if (state.Gamepad.wButtons != 0)
			{
				if (wasDown)
				{
						
					{
						keyCode = state.Gamepad.wButtons;

						mapp::GamepadKeyPressingEvent event = mapp::GamepadKeyPressingEvent(keyCode, i);
						mEventCallback(event);
					}
				}
				else
				{
						
					{
						wasDown = true;

						keyCode = state.Gamepad.wButtons;

						mapp::GamepadKeyPressedEvent event = mapp::GamepadKeyPressedEvent(keyCode, i);
						mEventCallback(event);
					}
				}
			}
			else if (wasDown)
			{
					
				{
					wasDown = false;

					mapp::GamepadKeyReleasedEvent event = mapp::GamepadKeyReleasedEvent(keyCode, i);
					mEventCallback(event);
				}
			}
		}
	}
}

bool WindowWin32::setFullscreen(const bool enable)
{
	bool isChangeSuccessful;

	HDC windowHDC = GetDC(mWindow);
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

		SetWindowLongPtr(mWindow, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		SetWindowLongPtr(mWindow, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(mWindow, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
		isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
		ShowWindow(mWindow, SW_MAXIMIZE);

		// Resize event
			
		{
			mapp::WindowResizeEvent event = mapp::WindowResizeEvent(fullscreenWidth, fullscreenHeight);
			mEventCallback(event);
		}
			
	}
	else
	{
		SetWindowLongPtr(mWindow, GWL_EXSTYLE, WS_EX_LEFT);
		SetWindowLongPtr(mWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
		SetWindowPos(mWindow, HWND_NOTOPMOST, getParams().mWidth, getParams().mHeight, getParams().mWidth + 0, getParams().mHeight + 0, SWP_SHOWWINDOW);
		ShowWindow(mWindow, SW_RESTORE);
	}

	mIsFullscreen = enable;

	return isChangeSuccessful;
}

}	// namespace mapp

#endif	// ifdef MAPP_PLATFORM_WIN32
