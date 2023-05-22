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

#pragma once

#include "mapp/platform.hpp"
#include "mapp/window.hpp"

#ifdef MAPP_PLATFORM_WIN32

#include <Windows.h>

namespace mapp {

class WindowWin32 : public Window
{
public:
	WindowWin32(const WindowParams& params);
	~WindowWin32();

	virtual void onUpdate(const float& dt) override;

	virtual bool setFullscreen(const bool enable) override;
	virtual bool getIsFullscreen() override { return mIsFullscreen; }

	virtual void* getNativeWindow() override { return mWindow; }
	virtual void* getNativeDisplay() override { return nullptr; }

private:
	const wchar_t* mClassName;
	HINSTANCE mInstance;
	HWND mWindow;
	bool mIsFullscreen;
};

}	// namespace mapp

#endif	// ifdef MAPP_PLATFORM_WIN32
